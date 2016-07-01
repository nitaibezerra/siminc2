<?php
/**
 * Navigation for mount menu
 *
 * @uses       Zend_View_Helper_Abstract
 * @package    Simec_View
 * @subpackage View\Helper
 */

class Simec_View_Helper_Navigation extends Zend_View_Helper_Abstract 
{
	private $_navigation = null;
	
	/**
	 * Menu Html
	 * 
	 * @var Html
	 */
	protected $_menu = null;
	
	/**
	 * List Html
	 *
	 * @var Html
	 */
	protected $_list = null;
	
	/**
	 * Menu HTML
	 * 
	 * @var HTML
	 */
	protected $_html = null;
	
	/**
	 * Main method
	 *
	 * @return HTML menu
	 */
	public function navigation() {
		if (Zend_Registry::isRegistered("navigation")) {
			$this->navigation = array ();
			$this->menu();
			$this->themes();
			$this->sistemas();
			$this->exercicio();
			return $this->navigation;
		}
	}
	
	public function menu() 
	{
		if (Zend_Registry::isRegistered("navigation")) 
		{
			$identity = Zend_Auth::getInstance()->getIdentity();

			$this->_menu = Zend_Registry::get("navigation");
			
			$this->_html = '<ul class="nav" id="side-menu">';
			
			$this->_html.= '<li class="nav-header" style="padding: 0px">';
			$this->_html.= '		<span>';
			$this->_html.= '			<div class="text-left">';
			$this->_html.= '				<img src="' . Zend_Controller_Front::getInstance()->getBaseUrl() . '/public/img/logo-simec.png" class="img-responsive" style="width: 80%; padding: 5px;">';
			$this->_html.= '			</div>';
			$this->_html.= '		</span>';
			$this->_html.= '</li>';
			$this->_html.= '<li class="nav-header" style="padding-top: 10px">';
			$this->_html.= '		<span>';
			$this->_html.= '			<div class="text-left">';
			$this->_html.= '				<img alt="image" class="text-center img-circle" src="' . Zend_Controller_Front::getInstance()->getBaseUrl() . '/public/img/profile_small.jpg">';
			$this->_html.= '			</div>';
			$this->_html.= '		</span>';
			$this->_html.= '		<span class="clear">'; 
			$this->_html.= '			<span class="block m-t-xs"> ';
			$this->_html.= '				<strong class="font-bold">' . $identity['auth']['usuario']['usunome'] . '</strong>';
			$this->_html.= '			</span>'; 
			$this->_html.= '		</span>';
			$this->_html.= '		<span style="color: #fff;" class="text-xs block"><b>' . $identity['auth']['usuario']['usufuncao'] . '</b></span>';
			$this->_html.= '		<div style="color: #fff; font-size: 11px">Sua sessão expira em: 59min21s</div>';
			$this->_html.= '	<div class="logo-element">';
			$this->_html.= '		SIMEC';
			$this->_html.= '	</div>';
			$this->_html.= '</li>';
				
			$links = $this->_menu[$_SESSION['sisid']];
			
			foreach ($links['menus'] as $nivel1) {
				if (count($nivel1['submenus']) > 0) {
					$this->_list.= '	<li>';
					$this->_list.= '		<a href="#' . $nivel1['mnudsc'] . '">';
					$this->_list.= '			<i class="fa fa-th-large"></i> ';
					$this->_list.= '			<span class="nav-label">' . $nivel1['mnudsc'] . '</span>';
					$this->_list.= '			<span class="fa arrow"></span>';
					$this->_list.= '		</a>';
					$this->_list.= '		<ul class="nav nav-second-level">';
					
					foreach ($nivel1['submenus'] as $nivel2) {
						if (count($nivel2['submenus']) > 0) {
							$this->_list.= '	<li>';
							$this->_list.= '		<a href="#' . $nivel2['mnudsc'] . '">';
							$this->_list.= '			<span class="nav-label">' . $nivel2['mnudsc'] . '</span>';
							$this->_list.= '			<span class="fa arrow"></span>';
							$this->_list.= '		</a>';
							$this->_list.= '		<ul class="nav nav-third-level">';
							
							foreach ($nivel2['submenus'] as $nivel3) {
								if (count($nivel3['submenus']) > 0) {
									$this->_list.= '	<li>';
									$this->_list.= '		<a href="#' . $nivel3['mnudsc'] . '">';
									$this->_list.= '			<span class="nav-label">' . $nivel3['mnudsc'] . '</span>';
									$this->_list.= '			<span class="fa arrow"></span>';
									$this->_list.= '		</a>';
									$this->_list.= '		<ul class="nav nav-fourth-level">';
									
									foreach ($nivel3['submenus'] as $nivel4) {
										$this->_list.= '		<li>';
										$this->_list.= '			<a href="' . $nivel4['mnulink'] . '">' . $nivel4['mnudsc'] . '</a>';
										$this->_list.= '		</li>';
									}
									
									$this->_list.= '		</uL>';
									
								} else {
									$this->_list.= '		<li>';
									$this->_list.= '			<a href="' . $nivel3['mnulink'] . '">' . $nivel3['mnudsc'] . '</a>';
									$this->_list.= '		</li>';
								}
							}
							
							$this->_list.= '		</uL>';
							
						} else {
							$this->_list.= '		<li>';
							$this->_list.= '			<a href="' . $nivel2['mnulink'] . '">' . $nivel2['mnudsc'] . '</a>';
							$this->_list.= '		</li>';
						}
					}
					
					$this->_list.= '		</uL>';
					
				} else {
					$this->_list.= '		<li>';
					$this->_list.= '			<a href="' . $nivel1['mnulink'] . '">';
					$this->_list.= '			<i class="fa fa-th-large"></i> ';
					$this->_list.= '			<span class="nav-label">' . $nivel1['mnudsc'] . '</span>';
					$this->_list.= '			</a>';
					$this->_list.= '		</li>';
				}
			}
			
			$this->_html.= $this->_list;

			$this->_html .= '</ul>';
			
			$this->navigation['menu'] = $this->_html;
		}
	}
	
	public function themes() {
		$this->navigation['themes'] = array('default', 'default-inverse', 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti');
	}
	
	public function sistemas() {
		$modelSistema = new Model_Seguranca_Sistema();
		
		$sistemas = new Zend_Session_Namespace ( 'Simec_Sistemas' );
		
		$this->navigation['sistemas'] = $modelSistema->getAll()->toArray();
	}
	
	public function exercicio() {
		$modelExericio = new Model_Public_Exercicio();
	
		$exercicio = new Zend_Session_Namespace ( 'Simec_Exercicio' );
	
		$exercicio->sisdiretorio = 'progorc';
		$exercicio->sisexercicio = true;
	
		$this->navigation['exercicio'] = $modelExericio->getExercicios($exercicio)->toArray();
	}
}
