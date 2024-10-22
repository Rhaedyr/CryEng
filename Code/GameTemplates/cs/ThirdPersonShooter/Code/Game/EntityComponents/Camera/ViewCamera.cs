﻿// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.


namespace CryEngine.Game
{
	[EntityComponent(Category = "Camera")]
	public class ViewCamera : EntityComponent
	{
		/// <summary>
		/// The currently active ViewCamera, or null of no ViewCamera is active at the moment.
		/// </summary>
		/// <value>The active camera.</value>
		public static ViewCamera ActiveCamera{ get; private set; }

		private View _view;
		private bool _active;

		/// <summary>
		/// Defines if this is the currently active <see cref="T:CryEngine.ViewCamera"/>. Only one <see cref="T:CryEngine.ViewCamera"/> can be active at a time.
		/// </summary>
		/// <value><c>true</c> if active camera; otherwise, <c>false</c>.</value>
		[EntityProperty(EntityPropertyType.Primitive, "Defines if this is the currently active camera. Only one camera can be active at a time.")]
		public bool Active
		{
			get
			{
				return _active;
			}
			set
			{
				if(value)
				{
					if(ActiveCamera != null)
					{
						ActiveCamera._active = false;
						ActiveCamera._view.SetActive(false);
					}
					_active = true;
					_view.SetActive(true);
					ActiveCamera = this;
				}
				else
				{
					if(ActiveCamera == this)
					{
						ActiveCamera = null;
					}
					_active = false;
					_view.SetActive(false);
				}
			}
		}

		protected override void OnInitialize()
		{
			base.OnInitialize();
			_view = View.Create();
			_view.LinkTo(Entity);

			if(_active || ActiveCamera == null)
			{
				Active = true;
			}
		}

		protected override void OnRemove()
		{
			base.OnRemove();

			if(_view != null)
			{
				Active = false;
				_view.RemoveView();
				_view = null;
			}
		}

		protected override void OnUpdate(float frameTime)
		{
			base.OnUpdate(frameTime);

			//Only call update when the view is actually active. 
			//The Update will return immediatly anyway if it's not active so we can save a managed->unmanaged call here.
			if(_active)
			{
				_view.Update(frameTime, _active);
			}
		}

		/// <summary>
		/// Links the Camera to an Entity by setting <paramref name="entity"/> as the parent of this Entity.
		/// If <paramref name="entity"/> is <c>null</c> the link will be removed.
		/// </summary>
		/// <param name="entity">Entity that the camera is linked to. If <c>null</c> it will remove the link from the camera.</param>
		/// <param name="resetPosition">If set to <c>true</c> the position will be reset to Vector3.Zero after linking to the Entity.</param>
		public void LinkCamera(Entity entity, bool resetPosition = false)
		{
			Entity.Parent = entity;
			if(resetPosition)
			{
				Entity.Position = Vector3.Zero;
			}
		}

		/// <summary>
		/// Unlinks the camera. Same as calling <c>LinkCamera(null)</c>.
		/// </summary>
		public void UnlinkCamera()
		{
			Entity.Parent = null;
		}
	}
}
