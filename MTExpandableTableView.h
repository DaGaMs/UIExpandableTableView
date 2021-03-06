//
//  MTExpandableTableView.h
//  iGithub
//
//  Created by Oliver Letterer on 11.04.11.
//  Copyright 2011 Home. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MTExpandableTableView;

typedef enum {
    MTExpansionStyleCollapsed = 0,
    MTExpansionStyleExpanded
} MTExpansionStyle;

@protocol MTExpandingTableViewCell <NSObject>

- (void)setLoading:(BOOL)loading;
- (void)setExpansionStyle:(MTExpansionStyle)style animated:(BOOL)animated;

@end



@protocol MTExpandableTableViewDatasource <UITableViewDataSource>

@required
- (BOOL)tableView:(MTExpandableTableView *)tableView canExpandSection:(NSInteger)section;
- (BOOL)tableView:(MTExpandableTableView *)tableView needsToDownloadDataForExpandableSection:(NSInteger)section;
- (UITableViewCell<MTExpandingTableViewCell> *)tableView:(MTExpandableTableView *)tableView expandingCellForSection:(NSInteger)section;

@optional
/** If the datasource implements this method, reloadDataAndResetExpansionStates:
  * will reset the section expansion accordingly. This is useful for swapping the
  * tableView organisation on the fly without the user noticing.
  */
- (BOOL)tableView:(MTExpandableTableView *)tableView expandSection:(NSInteger)section;
@end



@protocol MTExpandableTableViewDelegate <UITableViewDelegate>

@required
- (void)tableView:(MTExpandableTableView *)tableView downloadDataForExpandableSection:(NSInteger)section;

@optional
- (void)tableView:(MTExpandableTableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPathWhileAnimatingSection:(NSIndexPath *)indexPath;
- (void)tableView:(MTExpandableTableView *)tableView willExpandSection:(NSInteger)section;
- (void)tableView:(MTExpandableTableView *)tableView didExpandSection:(NSInteger)section;
- (void)tableView:(MTExpandableTableView *)tableView willCollapseSection:(NSInteger)section;
- (void)tableView:(MTExpandableTableView *)tableView didCollapseSection:(NSInteger)section;
@end



#ifndef MTExpandableTableView_weak
    #if __IPHONE_OS_VERSION_MIN_REQUIRED >= 50000
        #define MTExpandableTableView_weak weak
    #else 
        #define MTExpandableTableView_weak unsafe_unretained
    #endif
#endif

#ifndef __MTExpandableTableView_weak
    #if __IPHONE_OS_VERSION_MIN_REQUIRED >= 50000
        #define __MTExpandableTableView_weak __weak
    #else 
        #define __MTExpandableTableView_weak __unsafe_unretained
    #endif
#endif



@interface MTExpandableTableView : UITableView <UITableViewDelegate, UITableViewDataSource, NSCoding> {
@private
    id<UITableViewDelegate, MTExpandableTableViewDelegate> __MTExpandableTableView_weak _myDelegate;
    id<UITableViewDataSource, MTExpandableTableViewDatasource> __MTExpandableTableView_weak _myDataSource;
    
    NSMutableDictionary *_expandableSectionsDictionary;     // will store BOOLs for each section that is expandable
    NSMutableDictionary *_showingSectionsDictionary;        // will store BOOLs for the sections state (nil: not expanded, 1: expanded)
    NSMutableDictionary *_downloadingSectionsDictionary;    // will store BOOLs for the sections state (nil: not downloading, YES: downloading)
    NSMutableDictionary *_animatingSectionsDictionary;
    
    NSInteger _maximumRowCountToStillUseAnimationWhileExpanding;
    
    BOOL _onlyDisplayHeaderAndFooterViewIfTableViewIsNotEmpty;
    UIView *_storedTableHeaderView;
    UIView *_storedTableFooterView;
}

@property (nonatomic, MTExpandableTableView_weak) id<MTExpandableTableViewDelegate> delegate;

// discussion
// you wont receive any callbacks for row 0 in an expandable section anymore
@property (nonatomic, MTExpandableTableView_weak) id<MTExpandableTableViewDatasource> dataSource;

// discussion
// never use tableView.delegate/ tableView.dataSource as a getter. setDataSource will set _myDataSource, etc. so use these getters instead
@property (nonatomic, readonly, MTExpandableTableView_weak) id<MTExpandableTableViewDelegate> myDelegate;
@property (nonatomic, readonly, MTExpandableTableView_weak) id<MTExpandableTableViewDatasource> myDataSource;

@property (nonatomic, assign) NSInteger maximumRowCountToStillUseAnimationWhileExpanding;
/// This only controls duration of scrollView animation during row expansion/collapse.
/// Default is 0.25s.
@property (nonatomic, assign) CGFloat animationDuration;
@property (nonatomic, assign) BOOL onlyDisplayHeaderAndFooterViewIfTableViewIsNotEmpty;

// call tableView:needsToDownloadDataForExpandableSection: to make sure we can expand the section, otherwise through exception
- (void)expandSection:(NSInteger)section animated:(BOOL)animated;
- (void)collapseSection:(NSInteger)section animated:(BOOL)animated;
- (void)cancelDownloadInSection:(NSInteger)section;
- (void)toggleSection:(NSInteger)section;
/// Reset cached expansion and expandable states and reload from datasource.
/// @warning You need to call reloadData manually for these changes to take effect!
- (void)resetExpansionStates;
- (BOOL)canExpandSection:(NSInteger)section;
- (BOOL)isSectionExpanded:(NSInteger)section;

@end
