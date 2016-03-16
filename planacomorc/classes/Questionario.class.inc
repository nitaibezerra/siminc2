<?php


/**
 * Componente de listagem do novo layout.
 *
 * @see Simec_Listagem
 */
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class Questionario{
    
    /**
     * Objeto de Simec_Listagem
     * @var type 
     */
    private $simec_listagem;
    
    /**
     * Referente a Subações
     */
    const CAIXA_AZUL = 1;
    
    /**
     * Referente a Relatório de Gestão
     */
    const CAIXA_LARANJA = 2;
    
    /**
     * Referente a Ações
     */
    const CAIXA_VERDE = 3;
    
    
    private $apresenta_filtro = false;
       
    /**     
     * @param type $caixa. Utilizar uma das constantes para selecionar a qual caixa pertence o arquivo do questionario.
     */
    public function __construct($caixa) {
        $and = $_REQUEST['percod'] ? ' AND id_periodo_referencia = '.$_REQUEST['percod'] : '';
        self::javascript();
        $this->simec_listagem = new Simec_Listagem();
        switch ($caixa){
            case self::CAIXA_AZUL:                
                $this->apresenta_filtro = true;
                $this->subacoes($and);
                break;
            case self::CAIXA_LARANJA:                
                $this->relatorioGestao();
                break;
            case self::CAIXA_VERDE:                
                $this->apresenta_filtro = true;
                $this->acoes($and);
                break;
                
        }
        
    }
    
    public static function javascript(){
        echo <<<JAVASCRIPT
            <script>
                $(function(){
                    $('#percod').change(function() {
                        url = window.location.href;
                        url = url.replace(/&percod=[0-9]+/g,'');                        
                        window.location.href = url +"&percod=" + this.value;
                    });
                });
            </script>
JAVASCRIPT;
    }
    
    public function subacoes($and = ''){
        $this->simec_listagem->setCabecalho(array('Questionário','Número de caracteres previstos'));        
        $this->simec_listagem->setQuery("
            SELECT  
                qstid AS acao, qstnome AS nome, qstnumcaracteres AS car
            FROM planacomorc.monsubacaquestionario
            WHERE qststatus = 'A'
                $and
            ORDER BY qstid DESC
        ");
    }
    
    public function acoes($and = ''){
        $this->simec_listagem->setCabecalho(array('Questionário','Número de caracteres previstos'));
        $this->simec_listagem->setQuery("
            SELECT 
                qstid AS acao, qstnome AS nome, qstnumcaracteres AS car
            FROM planacomorc.monquestionario
            WHERE qststatus = 'A'
                $and
            ORDER BY qstid DESC
        ");
    }
    
    public function relatorioGestao(){
        $this->simec_listagem->setCabecalho(array("Questionário","Ano"));
        $this->simec_listagem->setQuery("
            SELECT
                tqtid AS acao, tqtdsc as nome, qtdexercicio
            FROM planacomorc.tcuquestionario
            WHERE tqtestado = 'A'
                AND qtdexercicio = '{$_SESSION['exercicio']}'
        ");
    }
    
    public function filtroPeriodo(){
        if($this->apresenta_filtro){
           
            $sql_periodo_referencia = <<<DML
                SELECT 
                    id_periodo_referencia AS codigo,
                    titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
                FROM planacomorc.periodo_referencia p
                WHERE id_exercicio = '{$_SESSION['exercicio']}'
                ORDER BY id_periodo_referencia desc
DML;
            $opcoesAdicionais = array('return' => true);
            $combo = inputCombo(
                'percod', $sql_periodo_referencia , $_REQUEST['percod'], 'percod', $opcoesAdicionais
            );
        
            echo <<<HTML
            <section class="form-group">
                <label class="control-label col-md-2" for="percod">Período</label>
                <section class="col-md-10">
                    {$combo}
                </section>                    
            </section> 
HTML;
        
        return;            
        }
        echo <<<HTML
            <section class="alert alert-info text-center">
                Não existem Filtros para esta página
            </section>
HTML;
        
    }
    
    /**
     * Renderizar Simec_Listagem
     * @param type $edit Nome da função javascript responsável por editar o questionário.
     * @param type $delete Nome da função javascript responsável por excluir o questionário.
     */
    public function renderListagem($edit = 'carregarQuestionario',$delete = 'excluirQuestionario'){
        $this->simec_listagem->addCallbackDeCampo(array('nome' ), 'alinhaParaEsquerda');
        $this->simec_listagem->turnOnPesquisator();
        $this->simec_listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $this->simec_listagem->addAcao('edit', $edit);
        $this->simec_listagem->addAcao('delete', $delete);
        $this->simec_listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}