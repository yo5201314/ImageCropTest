class UsersController < ApplicationController
  def index
    @users = User.all
  end

  def show
    @user = User.find(params[:id])
  end

  def new
    @user = User.new
  end

  def create
    @user = User.new(params[:user])
    if @user.save
      if params[:user][:avatar].present?
        render :crop
      else
        redirect_to @user, notice: "Successfully created image."
      end
    else
      render :new
    end
  end

  def edit
    @user = User.find(params[:id])
  end

  def update
    @user = User.find(params[:id])
    if @user.update_attributes(params[:user])
      if params[:user][:avatar].present?
        render :crop
      else
        redirect_to @user, notice: "Successfully updated image."
      end
    else
      render :new
    end
  end

  def destroy
    @user = User.find(params[:id])
    @user.destroy
    redirect_to users_url, notice: "Successfully destroyed image."
  end
end
