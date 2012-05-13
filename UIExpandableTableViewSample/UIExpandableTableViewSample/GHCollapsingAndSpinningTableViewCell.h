//
//  UICollapsingTableViewCell.h
//  iGithub
//
//  Created by me on 04.04.11.
//  Copyright 2011 Home. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MTExpandableTableView.h"

@interface GHCollapsingAndSpinningTableViewCell : UITableViewCell <MTExpandingTableViewCell> {
@private
    BOOL _isSpinning;
    
    UIActivityIndicatorView *_activityIndicatorView;
    UIImageView *_disclosureIndicatorImageView;
}

@property (nonatomic, retain) UIActivityIndicatorView *activityIndicatorView;
@property (nonatomic, retain) UIImageView *disclosureIndicatorImageView;

- (void)setSpinning:(BOOL)spinning;

@end
