class User < ActiveRecord::Base
  mount_uploader :avatar, AvatarUploader
  attr_accessor :crop_x, :crop_y, :crop_w, :crop_h
  after_update :crop_image
  
  def crop_image
    avatar.recreate_versions! if crop_x.present?
  end
end
