<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class Acomporc_Service_Gerenciador
{
    const TIPO_ACAO = 'A';
    const TIPO_SUBACAO = 'S';
    const TIPO_TCU = 'T';

    const CONTROLLER_ACOMPANHAMENTO_SUBACAO = 1;
    const CONTROLLER_ASSOCIACAO_ASSOCIACAO = 2;
    const CONTROLLER_PERIODO_PERIODO = 3;
    const CONTROLLER_QUESTIONARIO_QUESTAO = 4;
    const CONTROLLER_QUESTIONARIO_QUESTIONARIO = 5;
    const CONTROLLER_ACOMPANHAMENTO_CENTRAL = 6;
    const CONTROLLER_RELATORIOGESTAO_RELATORIO = 7;
    const CONTROLLER_ACOMPANHAMENTO_ACAO = 8;

    private $breadcrumbName = '';
    private $breadcrumbNameController = '';
    private $url = array();
    private $controller = '';
    private $type = '';
    private $typeName = '';
    private $param = '';

    public function __get($name)
    {
        switch($name){
            case 'typeName':
                return $this->typeName;
        }
    }

    public function __construct($controller = self::PAGINA_ACOMPANHAMENTO_ACOMPANHAMENTO, $type = self::TIPO_ACAO)
    {
        $this->_checkController($controller);
        $this->_checkType($type);
    }

    protected function _checkController($controller = self::PAGINA_ACOMPANHAMENTO_ACOMPANHAMENTO)
    {
        switch($controller)
        {
            case self::CONTROLLER_ACOMPANHAMENTO_SUBACAO:
                self::acompanhamentoSubacao();
                break;
            case self::CONTROLLER_ASSOCIACAO_ASSOCIACAO:
                self::associacao();
                break;
            case self::CONTROLLER_PERIODO_PERIODO:
                self::periodo();
                break;
            case self::CONTROLLER_QUESTIONARIO_QUESTAO:
                self::questao();
                break;
            case self::CONTROLLER_QUESTIONARIO_QUESTIONARIO:
                self::questionario();
                break;
            case self::CONTROLLER_ACOMPANHAMENTO_CENTRAL:
                self::central();
                break;
            case self::CONTROLLER_RELATORIOGESTAO_RELATORIO:
                self::relatorioTCU();
                break;
            case self::CONTROLLER_ACOMPANHAMENTO_ACAO:
                self::acompanhamentoAcao();
                break;
            default:
                throw new Exception('Preencha o nome da controller antes de prosseguir');
        }
    }

    private function acompanhamentoSubacao()
    {
        $this->controller = 'subacao';
        $this->breadcrumbNameController = '';
        $this->url = array('ini' => 'formFiltroAcompanhamento.inc', 'alt' => array(
            'A' => '',
            'S' => 'formAcompanhamento.inc',
            'T' => ''));
    }

    private function acompanhamentoAcao()
    {
        $this->controller = 'acao';
        $this->breadcrumbNameController = '';
        $this->url = array('ini' => 'formFiltroAcao.inc', 'alt' => 'formAcao.inc');
    }

    private function associacao()
    {
        $this->controller = 'associa';
        $this->breadcrumbNameController = 'Associação';
        $this->url = array('ini' => 'formFiltroAssociacao.inc', 'alt' => array(
            'A' => 'formAssociacaoAcao.inc',
            'S' => 'formAssociacaoSubacao.inc',
            'T' => 'formAssociacaoRelatorioTCU.inc'));
    }

    private function periodo()
    {
        $this->controller = 'periodo';
        $this->breadcrumbNameController = 'Período de Acompanhamento';
        $this->url = array('ini' => 'formFiltroPeriodo.inc', 'alt' => 'formPeriodo.inc');
    }

    private function questao()
    {
        $this->controller = 'questao';
        $this->breadcrumbNameController = 'Questões';
        $this->url = array('ini' => 'formQuestaoFiltro.inc', 'alt' => 'formQuestao.inc');
    }

    private function questionario()
    {
        $this->controller = 'questionario';
        $this->breadcrumbNameController = 'Questionário';
        $this->url = array('ini' => 'formQuestionarioFiltro.inc', 'alt' => 'formQuestionario.inc');
    }

    private function central()
    {
        $this->controller = 'central';
        $this->breadcrumbNameController = 'Central de Acompanhamento';
        $this->url = array('ini' => 'formFiltroCentral.inc', 'alt' => '');
    }

    private function relatorioTCU()
    {
        $this->controller = 'relatorio';
        $this->breadcrumbNameController = 'Relatório';
        $this->url = array('ini' => 'formFiltroRelatorio.inc', 'alt' => 'formRelatorio.inc');
    }


    protected function _checkType($type = self::TIPO_ACAO)
    {
        switch ($type)
        {
            case self::TIPO_ACAO:
                $this->type = self::TIPO_ACAO;
                $this->typeName = 'Ações';
                $this->breadcrumbName = 'Acompanhamento de Ações';
            break;
            case self::TIPO_SUBACAO:
                $this->type = self::TIPO_SUBACAO;
                $this->typeName = 'Subações';
                $this->breadcrumbName = 'Acompanhamento de Subações';
            break;
            case self::TIPO_TCU:
                $this->type = self::TIPO_TCU;
                $this->typeName = 'Relatório TCU';
                $this->breadcrumbName = 'Relatório de Gestão (TCU/CGU)';
            break;
            default:
                $this->breadcrumbName = 'Seleção de Tipo';
                break;
        }
    }

    public function getBreadcrumb()
    {
        $extra = '';
        if($this->breadcrumbNameController != ''){
            $extra = "<li class=\"{$this->breadcrumbNameController}\">{$this->breadcrumbNameController}</li>";
        }
        $retorno =  "
        <ol class=\"breadcrumb\">
            <li><a href=\"acomporc.php?modulo=inicio&acao=C\">Acompanhamento Orçamentário</a></li>
            <li>{$this->breadcrumbName}</li>
            ".$extra."
            ".$this->_getBreadCrumbDescription()."
        </ol>
        ";
        return $retorno;
    }

    protected function _isRequisicao()
    {
        return isset($_GET['requisicao']) || !empty($_GET['requisicao']);
    }

    protected function _getBreadCrumbDescription()
    {
        if($this->_isRequisicao()){
            switch($_GET['requisicao']){
                case 'acessar':
                    return '<li class="active">%s - %s</li>';
                case 'new':
                    return '<li class="active">Cadastro</li>';
                case 'update':
                    return '<li class="active">Atualização</li>';
                case 'relatorio':
                    return '<li class="active">Dados da Ação</li>';
            }
        }
        return '';
    }

    public function getURL()
    {
        $url = $this->controller . "/" . ($this->_isRequisicao()
                                            ?(is_array($this->url['alt'])
                                                ? $this->url['alt'][$this->type]
                                                :$this->url['alt'])
                                            : $this->url['ini']);
        return $url;
    }

    public function addParams($data)
    {
        $this->param = '';
        foreach($data as $key => $value){
            if(trim($key) != '' && trim($value) != ''){
                $this->param .= "&$key=$value";
            }
        }
    }

    public function redirect($address,$manterURL = false)
    {
        if($manterURL){
            $this->param = '';
            $data = $_GET;
            unset($data['modulo']);
            unset($data['acao']);
            $this->addParams($data);
        }
        $url = $address.$this->param;
        echo "<script>window.location.assign(\"{$url}\")</script>";
        die();
    }
}