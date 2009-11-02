#include "../include/renderer_impl.h"

#include "../include/shaderregs_op.h"
#include "../include/clipper.h"
#include "../include/resource_manager.h"
#include "../include/geometry_assembler.h"
#include "../include/rasterizer.h"
#include "../include/framebuffer.h"
#include "../include/stream.h"
#include "../include/surface.h"

using namespace efl;

//inherited
result renderer_impl::set_input_layout(const input_layout_decl& layout)
{
	hga_->set_input_layout(layout);
	return result::ok;
}

const input_layout_decl& renderer_impl::get_input_layout() const
{
	NO_IMPL();
	static input_layout_decl ret;
	return ret;
}

result renderer_impl::set_stream(stream_index sidx, h_buffer hbuf)
{
	hga_->set_stream(sidx, hbuf);
	return result::ok;
}

h_buffer renderer_impl::get_stream(stream_index /*sidx*/) const
{
	NO_IMPL();
	return h_buffer();
}

result renderer_impl::set_index_buffer(h_buffer hbuf, index_type idxtype)
{
	hga_->set_index_buffer(hbuf, idxtype);
	return result::ok;
}

h_buffer renderer_impl::get_index_buffer() const
{
	NO_IMPL();
	return h_buffer();
}

index_type renderer_impl::get_index_type() const
{
	NO_IMPL();
	return index_int16;
}

//
result renderer_impl::set_primitive_topology(primitive_topology primtopo)
{
	hga_->set_primitive_topology(primtopo);
	return result::ok;
}

primitive_topology renderer_impl::get_primitive_topology() const
{
	NO_IMPL();
	return primitive_triangle_list;
}

result renderer_impl::set_vertex_shader(h_vertex_shader hvs)
{
	hvs_ = hvs;
	return result::ok;
}

h_vertex_shader renderer_impl::get_vertex_shader() const
{
	return hvs_;
}

result renderer_impl::set_cull_mode(cull_mode cm)
{
	hrast_->set_cull_mode(cm);
	return result::ok;
}

cull_mode renderer_impl::get_cull_mode() const
{
	NO_IMPL();
	return cull_none;
}

result renderer_impl::set_fill_mode(fill_mode fm)
{
	hrast_->set_fill_mode(fm);
	return result::ok;
}

fill_mode renderer_impl::get_fill_mode() const
{
	NO_IMPL();
	return fill_solid;
}

result renderer_impl::set_pixel_shader(h_pixel_shader hps)
{
	hps_ = hps;
	hrast_->on_pixel_shader_updated();
	return result::ok;
}

h_pixel_shader renderer_impl::get_pixel_shader() const
{
	return hps_;
}

result renderer_impl::set_blend_shader(h_blend_shader hbs)
{
	hbs_ = hbs;
	return result::ok;
}

h_blend_shader renderer_impl::get_blend_shader()
{
	return hbs_;
}

result renderer_impl::set_viewport(const viewport& vp)
{
	vp_ = vp;
	return result::ok;
}

const viewport& renderer_impl::get_viewport() const
{
	return vp_;
}

result renderer_impl::set_framebuffer_size(size_t width, size_t height)
{
	hfb_->reset(width, height, hfb_->get_buffer_format());
	return result::ok;
}

efl::rect<size_t> renderer_impl::get_framebuffer_size() const
{
	return hfb_->get_rect();
}

//
result renderer_impl::set_framebuffer_format(pixel_format pxfmt)
{
	hfb_->reset(hfb_->get_width(), hfb_->get_height(), pxfmt);
	return result::ok;
}

pixel_format renderer_impl::get_framebuffer_format(pixel_format /*pxfmt*/) const
{
	return hfb_->get_buffer_format();
}

result renderer_impl::set_render_target_available(render_target tar, size_t tar_id, bool valid)
{
	if(valid){
		hfb_->set_render_target_enabled(tar, tar_id);
	} else {
		hfb_->set_render_target_disabled(tar, tar_id);
	}

	return result::ok;
}

bool renderer_impl::get_render_target_available(render_target /*tar*/, size_t /*tar_id*/) const
{
	NO_IMPL();
	return false;
}

//do not support get function for a while
result renderer_impl::set_render_target(render_target tar, size_t tar_id, surface* psurf)
{
	hfb_->set_render_target(tar, tar_id, psurf);
	return result::ok;
}

h_renderer_mementor renderer_impl::create_mementor()
{
	return h_renderer_mementor();
}

result renderer_impl::release_mementor(h_renderer_mementor& /*mementor*/)
{
	NO_IMPL();
	return result::ok;
}

result renderer_impl::set_additional_state(const boost::any& /*state*/)
{
	return result::ok;
}

boost::any renderer_impl::get_additional_state(const boost::any& /*name*/)
{
	return boost::any();
}

//
h_buffer renderer_impl::create_buffer(size_t size)
{
	return hbufmgr_->create_buffer(size);
}

result renderer_impl::release_buffer(h_buffer& hbuf)
{
	hbufmgr_->release_buffer(hbuf);
	return result::ok;
}

h_texture renderer_impl::create_tex2d(size_t width, size_t height, pixel_format fmt)
{
	return htexmgr_->create_texture_2d(width, height, fmt);
}

h_texture renderer_impl::create_texcube(size_t width, size_t height, pixel_format fmt)
{
	return htexmgr_->create_texture_cube(width, height, fmt);
}

result renderer_impl::release_texture(h_texture& htex)
{
	htexmgr_->release_texture(htex);
	return result::ok;
}

h_sampler renderer_impl::create_sampler()
{
	return h_sampler(new sampler);
}

result renderer_impl::release_sampler(h_sampler& hsmp)
{
	hsmp.reset();
	return result::ok;
}

result renderer_impl::draw(size_t startpos, size_t primcnt)
{
	hga_->draw(startpos, primcnt);
	return result::ok;
}

result renderer_impl::draw_index(size_t startpos, size_t primcnt, int basevert)
{
	hga_->draw_index(startpos, primcnt, basevert);
	return result::ok;
}

result renderer_impl::clear_color(size_t tar_id, const color_rgba32f& c)
{
	hfb_->clear_color(tar_id, c);
	return result::ok;
}

result renderer_impl::clear_depth(float d)
{
	hfb_->clear_depth(d);
	return result::ok;
}

result renderer_impl::clear_stencil(uint32_t s)
{
	hfb_->clear_stencil(s);
	return result::ok;
}

result renderer_impl::clear_color(size_t tar_id, const efl::rect<size_t>& rc, const color_rgba32f& c)
{
	hfb_->clear_color(tar_id, rc, c);
	return result::ok;
}

result renderer_impl::clear_depth(const efl::rect<size_t>& rc, float d)
{
	hfb_->clear_depth(rc, d);
	return result::ok;
}

result renderer_impl::clear_stencil(const efl::rect<size_t>& rc, uint32_t s)
{
	hfb_->clear_stencil(rc, s);
	return result::ok;
}

result renderer_impl::present()
{
	NO_IMPL();
	return result::ok;
}

void renderer_impl::initialize(){
	hga_->initialize(this);
	hrast_->initialize(this);
	hfb_->initialize(this);
}

renderer_impl::renderer_impl(const renderer_parameters* pparam, h_device hdev)
{
	hbufmgr_.reset(new buffer_manager());
	htexmgr_.reset(new texture_manager());

	hga_.reset(new geometry_assembler());
	hclipper_.reset(new clipper());
	hrast_.reset(new rasterizer());
	hfb_.reset(
		new framebuffer(
		pparam->backbuffer_width,
		pparam->backbuffer_height,
		pparam->backbuffer_format
		)
		);
	hdev_ = hdev;

	vp_.minz = 0.0f;
	vp_.maxz = 1.0f;
	vp_.w = pparam->backbuffer_width;
	vp_.h = pparam->backbuffer_height;
	vp_.x = vp_.y = 0;

	initialize();
}

h_buffer_manager renderer_impl::get_buf_mgr()
{
	return hbufmgr_;
}

h_texture_manager renderer_impl::get_tex_mgr()
{
	return htexmgr_;
}

h_geometry_assembler renderer_impl::get_geometry_assembler()
{
	return hga_;
}

h_rasterizer renderer_impl::get_rasterizer()
{
	return hrast_;
}

h_framebuffer renderer_impl::get_framebuffer()
{
	return hfb_; 
}

h_device renderer_impl::get_device()
{
	return hdev_;
}

h_vertex_cache renderer_impl::get_vertex_cache()
{
	return hvertcache_;
}

h_clipper renderer_impl::get_clipper()
{
	return hclipper_;
}

h_renderer create_software_renderer(const renderer_parameters* pparam, h_device hdev)
{
	return h_renderer(new renderer_impl(pparam, hdev));
}