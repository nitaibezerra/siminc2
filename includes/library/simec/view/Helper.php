<?php

set_include_path(get_include_path() . PATH_SEPARATOR . APPRAIZ . DIRECTORY_SEPARATOR . 'zimec' . DIRECTORY_SEPARATOR . 'library');

/**
 * Abstract class for extension
 */
require_once APPRAIZ . 'zimec/library/Simec/Util.php';
require_once APPRAIZ . 'zimec/library/Zend/Date.php';
require_once APPRAIZ . 'zimec/library/Zend/View.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Element.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Title.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Input.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Data.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Cep.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Email.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Cpf.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Cnpj.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Telefone.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Select.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Textarea.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Options.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Radio.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Checkbox.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Tab.php';

class Simec_View_Helper
{
    const K_FORM_TIPO_INLINE = 'INLINE';
    const K_FORM_TIPO_VERTICAL = 'VERTICAL';
    const K_FORM_TIPO_HORIZONTAL = 'HORIZONTAL';

	private static $view;
	private static $errorValidate = array();

    protected $formTipo = self::K_FORM_TIPO_HORIZONTAL;
	
	public function __construct()
    {
        $this->errorValidate = isset($_SESSION['form_validate']['erros']) ? $_SESSION['form_validate']['erros'] : array();
        unset($_SESSION['form_validate']['erros']);

		if (!Simec_View_Helper::$view) {
			Simec_View_Helper::$view = new Zend_View();
		}

		$this->title = new Simec_View_Helper_Title();
		$this->boolean = new Simec_View_Helper_Boolean();
		$this->input = new Simec_View_Helper_Input();
		$this->data = new Simec_View_Helper_Data();
		$this->cpf = new Simec_View_Helper_Cpf();
		$this->cnpj = new Simec_View_Helper_Cnpj();
		$this->cep = new Simec_View_Helper_Cep();
		$this->email = new Simec_View_Helper_Email();
		$this->telefone = new Simec_View_Helper_Telefone();
		$this->select = new Simec_View_Helper_Select();
		$this->textarea = new Simec_View_Helper_Textarea();
		$this->checkbox = new Simec_View_Helper_Checkbox();
		$this->radio = new Simec_View_Helper_Radio();
		$this->tab = new Simec_View_Helper_Tab();
	}

    /**
     * @return string
     */
    public function getFormTipo()
    {
        return $this->formTipo;
    }

    /**
     * @param string $formTipo
     */
    public function setFormTipo($formTipo)
    {
        $this->formTipo = $formTipo;
    }

	public function title($title, $subTitle = null, $attribs = array())
    {
		$this->title->setView(Simec_View_Helper::$view);
		return $this->title->title($title, $subTitle, $attribs);
	}
	
	public function boolean($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $config = $this->montarConfig($name, $config);
		$this->boolean->setView(Simec_View_Helper::$view);
		return $this->boolean->boolean($name, $label, $value, $attribs, $config);
	}

	public function input($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $config = $this->montarConfig($name, $config);
		$this->input->setView(Simec_View_Helper::$view);
		return $this->input->input($name, $label, $value, $attribs, $config);
	}

	public function valor($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $attribs['class'] = $attribs['class'] . ' valor ';
        $value = is_numeric($value) ? number_format($value, 2, ',', '.') : null;

        $config = $this->montarConfig($name, $config);
		$this->input->setView(Simec_View_Helper::$view);
		return $this->input->input($name, $label, $value, $attribs, $config);
	}

	public function cep($name, $label = null, $value = null, $attribs = array(), $config = array())
	{
		$config = $this->montarConfig($name, $config);
		$this->cep->setView(Simec_View_Helper::$view);
		return $this->cep->cep($name, $label, $value, $attribs, $config);
	}
	
	public function cpf($name, $label = null, $value = null, $attribs = array(), $config = array())
	{
		$config = $this->montarConfig($name, $config);
		$this->cpf->setView(Simec_View_Helper::$view);
		return $this->cpf->cpf($name, $label, $value, $attribs, $config);
	}

	public function cnpj($name, $label = null, $value = null, $attribs = array(), $config = array())
	{
		$config = $this->montarConfig($name, $config);
		$this->cnpj->setView(Simec_View_Helper::$view);
		return $this->cnpj->cnpj($name, $label, $value, $attribs, $config);
	}
	
	public function email($name, $label = null, $value = null, $attribs = array(), $config = array())
	{
		$config = $this->montarConfig($name, $config);
		$this->email->setView(Simec_View_Helper::$view);
		return $this->email->email($name, $label, $value, $attribs, $config);
	}
	
	public function telefone($name, $label = null, $value = null, $attribs = array(), $config = array())
	{
		$config = $this->montarConfig($name, $config);
		$this->telefone->setView(Simec_View_Helper::$view);
		return $this->telefone->telefone($name, $label, $value, $attribs, $config);
	}
	
	public function data($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $config = $this->montarConfig($name, $config);
		$this->data->setView(Simec_View_Helper::$view);
		return $this->data->data($name, $label, $value, $attribs, $config);
	}

	public function select($name, $label = null, $value = null, $options = array(), $attribs = null, $config = array())
    {
        $config = $this->montarConfig($name, $config);
        if(is_string($options)){
            global $db;
            $options = $db->carregar($options);
            $options = simec_preparar_array($options);
        }
		$this->select->setView(Simec_View_Helper::$view);
		return $this->select->select($name, $label, $value, $options, $attribs, $config);
	}

	public function textarea($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $config = $this->montarConfig($name, $config);
		$this->textarea->setView(Simec_View_Helper::$view);
		return $this->textarea->textarea($name, $label, $value, $attribs, $config);
	}
	
	public function radio($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
    {
        $config = $this->montarConfig($name, $config);
		$this->radio->setView(Simec_View_Helper::$view);
		return $this->radio->radio($name, $label, $value, $options, $attribs, $config);
	}
	
	public function checkbox($name, $label = null, $value = null, $options = null, $attribs = array(), $config = array())
    {
        $config = $this->montarConfig($name, $config);
		$this->checkbox->setView(Simec_View_Helper::$view);
		return $this->checkbox->checkbox($name, $label, $value, $options, $attribs, $config);
	}
	
	public function tab($itens = array(), $url = false, $config = array())
    {
		return $this->tab->tab($itens, $url, $config);
	}

	protected function montarConfig($name, $config = array())
    {
        if(empty($config['formTipo'])){
            $config['formTipo'] = $this->formTipo;
        }

        $config['errorValidate'] = (is_array($this->errorValidate) && !empty($this->errorValidate[$name])) ? $this->errorValidate[$name] : array();
        return $config;
	}
}