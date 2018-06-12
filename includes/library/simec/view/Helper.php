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
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Periodo.php';
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
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Municipios.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Transfer.php';
require_once APPRAIZ . 'zimec/library/Simec/View/Helper/Time.php';

class Simec_View_Helper {

    const K_FORM_TIPO_INLINE = 'INLINE';
    const K_FORM_TIPO_VERTICAL = 'VERTICAL';
    const K_FORM_TIPO_HORIZONTAL = 'HORIZONTAL';

    private static $view;
    private static $errorValidate = array();
    protected $formTipo = self::K_FORM_TIPO_HORIZONTAL;
    protected $labelSize;
    protected $inputSize;
    protected $podeEditar = true;

    public function __construct() {
        self::$errorValidate = isset($_SESSION['form_validate']['erros']) ? $_SESSION['form_validate']['erros'] : array();
        unset($_SESSION['form_validate']['erros']);

        if (!Simec_View_Helper::$view) {
            Simec_View_Helper::$view = new Zend_View();
            Simec_View_Helper::$view->setEncoding('ISO-8859-1');
        }

        $this->title = new Simec_View_Helper_Title();
        $this->radioBoolean = new Simec_View_Helper_RadioBoolean();
        $this->input = new Simec_View_Helper_Input();
        $this->data = new Simec_View_Helper_Data();
        $this->periodo = new Simec_View_Helper_Periodo();
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
        $this->municipios = new Simec_View_Helper_Municipios();
        $this->transfer = new Simec_View_Helper_Transfer();
        $this->time = new Simec_View_Helper_Time();
    }

    /**
     * @return string
     */
    public function getFormTipo() {
        return $this->formTipo;
    }

    /**
     * @param string $formTipo
     */
    public function setFormTipo($formTipo) {
        $this->formTipo = $formTipo;
    }

    /**
     * @return mixed
     */
    public function getLabelSize() {
        return $this->labelSize;
    }

    /**
     * @param mixed $labelSize
     */
    public function setLabelSize($labelSize) {
        $this->labelSize = $labelSize;
    }

    /**
     * @return mixed
     */
    public function getInputSize() {
        return $this->inputSize;
    }

    /**
     * @param mixed $inputSize
     */
    public function setInputSize($inputSize) {
        $this->inputSize = $inputSize;
    }

    /**
     * @return mixed
     */
    public function getPodeEditar() {
        return $this->podeEditar;
    }

    /**
     * @param mixed $podeEditar
     */
    public function setPodeEditar($podeEditar) {
        $this->podeEditar = $podeEditar;
    }

    public function title($title, $subTitle = null, $attribs = array()) {
        $this->title->setView(Simec_View_Helper::$view);
        return $this->title->title($title, $subTitle, $attribs);
    }

    public function radioBoolean($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->radioBoolean->setView(Simec_View_Helper::$view);
        return $this->radioBoolean->radioBoolean($name, $label, $value, $attribs, $config);
    }

    public function input($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->input->setView(Simec_View_Helper::$view);
        return $this->input->input($name, $label, $value, $attribs, $config);
    }

    public function valor($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $attribs['class'] = $attribs['class'] . ' valor ';
        $value = is_numeric($value) ? number_format($value, 2, ',', '.') : null;

        $config = $this->montarConfig($name, $config);
        $this->input->setView(Simec_View_Helper::$view);
        return $this->input->input($name, $label, $value, $attribs, $config);
    }

    /**
     * Método para uso de valores inteiros.
     *
     * @param
     */
    public function moedaInteiro($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $attribs['class'] = $attribs['class'] . ' moedaInteiro ';
        $value = is_numeric($value) ? number_format($value, 0, ',', '.') : null;

        $config = $this->montarConfig($name, $config);
        $this->input->setView(Simec_View_Helper::$view);
        return $this->input->input($name, $label, $value, $attribs, $config);
    }

    public function cep($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->cep->setView(Simec_View_Helper::$view);
        return $this->cep->cep($name, $label, $value, $attribs, $config);
    }

    public function cpf($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->cpf->setView(Simec_View_Helper::$view);
        return $this->cpf->cpf($name, $label, $value, $attribs, $config);
    }

    public function cnpj($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->cnpj->setView(Simec_View_Helper::$view);
        return $this->cnpj->cnpj($name, $label, $value, $attribs, $config);
    }

    public function email($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->email->setView(Simec_View_Helper::$view);
        return $this->email->email($name, $label, $value, $attribs, $config);
    }

    public function telefone($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->telefone->setView(Simec_View_Helper::$view);
        return $this->telefone->telefone($name, $label, $value, $attribs, $config);
    }

    public function data($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->data->setView(Simec_View_Helper::$view);
        return $this->data->data($name, $label, $value, $attribs, $config);
    }

    public function periodo($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->periodo->setView(Simec_View_Helper::$view);
        return $this->periodo->periodo($name, $label, $value, $attribs, $config);
    }

    public function select($name, $label = null, $value = null, $options = array(), $attribs = null, $config = array()) {
        $config = $this->montarConfig($name, $config);

        if (is_array($options) && 3 == count($options) && false !== strpos($options[0], '.')) {
            $options = "select {$options[1]} as codigo, {$options[2]} as descricao from {$options[0]} order by descricao";
        }

        if (is_string($options)) {
            global $db;
            $options = $db->carregar($options);
            $options = $options ? $options : array();
            $options = simec_preparar_array($options);
        }
        $this->select->setView(Simec_View_Helper::$view);
        return $this->select->select($name, $label, $value, $options, $attribs, $config);
    }

    public function textarea($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->textarea->setView(Simec_View_Helper::$view);
        return $this->textarea->textarea($name, $label, $value, $attribs, $config);
    }

    public function radio($name, $label = null, $value = null, $options = null, $attribs = null, $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->radio->setView(Simec_View_Helper::$view);
        return $this->radio->radio($name, $label, $value, $options, $attribs, $config);
    }

    public function checkbox($name, $label = null, $value = null, $options = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->checkbox->setView(Simec_View_Helper::$view);
        return $this->checkbox->checkbox($name, $label, $value, $options, $attribs, $config);
    }

    public function uf($name, $label = null, $value = null, $options = array(), $attribs = null, $config = array()) {
        $nameUf = is_array($name[0]) && !empty($name[0]) ? $name[0] : 'estuf';
        $labelUf = is_array($label[0]) && !empty($label[0]) ? $label[0] : 'UF';
        $valueUf = is_array($value[0]) && !empty($value[0]) ? $value[0] : null;

        $config = $this->montarConfig($nameUf, $config);
        $this->input->setView(Simec_View_Helper::$view);
        return $this->input->input($nameUf, $labelUf, $valueUf, $attribs, $config);
    }

    public function tab($itens = array(), $url = false, $config = array()) {
        return $this->tab->tab($itens, $url, $config);
    }

    public function municipios($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->input->setView(Simec_View_Helper::$view);
        return $this->municipios->municipios($name, $label, $value, $attribs, $config);
    }

    public function transfer($name, $label = null, $optionsAvailable = null, $optionsSelected = array(), $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->transfer->setView(Simec_View_Helper::$view);
        return $this->transfer->transfer($name, $label, $optionsAvailable, $optionsSelected, $attribs, $config);
    }

    public function time($name, $label = null, $value = null, $attribs = array(), $config = array()) {
        $config = $this->montarConfig($name, $config);
        $this->time->setView(Simec_View_Helper::$view);
        return $this->time->time($name, $label, $value, $attribs, $config);
    }

    protected function montarConfig($name, $config = array()) {
        if (empty($config['formTipo'])) {
            $config['formTipo'] = $this->formTipo;
        }

        if (empty($config['pode-editar'])) {
            $config['pode-editar'] = $this->podeEditar;
        }

        $config['label-size'] = (empty($config['label-size']) && $this->labelSize) ? $this->labelSize : $config['label-size'];
        $config['input-size'] = (empty($config['input-size']) && $this->inputSize) ? $this->inputSize : $config['input-size'];

        if (!is_array($name)) {
            $config['errorValidate'] = (is_array(self::$errorValidate) && !empty(self::$errorValidate[$name])) ? self::$errorValidate[$name] : array();
        }
        return $config;
    }

    public function limparNumero($num) {
        for ($x = 0; $x < strlen($num); $x++) {
            if (ereg("[0-9]", $num[$x]))
                $saida .= $num[$x];
        }
        return $saida;
    }

    public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = array(), $descricaoCampo = null) {
        $html = '';
        $selected = '';

        if (isset($htmlOptions['prompt'])) {
            $html .= '<option value="">' . strtr($htmlOptions['prompt'], array('<' => '&lt;', '>' => '&gt;')) . "</option>\n";
        }

        if ($dados) {
            foreach ($dados as $data) {
                if ($idCampo) {
                    $selected = ( in_array($data['codigo'], $idCampo) ? "selected='true' " : "");
                }
                $html .= "<option {$selected}  title=\"{$data['descricao']}\" value=\"" . $data['codigo'] . "\">  " . simec_htmlentities($data['descricao']) . " </option> ";
            }
        }
        return $html;
    }

}
