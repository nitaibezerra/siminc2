<?php
/**
 * Acesso ao Webservice de Alterações Financeiras.
 * $Id: WSAlteracoesOrcamentarias.php 101880 2015-08-31 19:50:33Z maykelbraz $
 */

/**
 * Base dos webservices da Sof.
 * @see Spo_Ws_Sof
 */
require(APPRAIZ . 'spo/ws/Sof.php');

/**
 * Classes de mapeamento dos tipos do serviço.
 * @see WSAlteracoesOrcamentariasMap.php
 */
require(dirname(__FILE__) . '/WSAlteracoesOrcamentariasMap.php');

/**
 * Chamadas ao WSAlteracoesOrcamentarias.
 */
class WSAlteracoesOrcamentarias extends Spo_Ws_Sof
{
    public function __construct($env = null)
    {
        parent::__construct('altorc', $env);
    }

    /**
     * URL de acesso ao webservice.
     * @return \WSAlteracoesOrcamentarias
     */
    protected function loadURL()
    {
        $this->urlWSDL = WEB_SERVICE_SIOP_URL. 'WSAlteracoesOrcamentarias?wsdl';
        return $this;
    }

    /**
     * {@inheritdoc}
     */
    protected function loadClassMap()
    {
    	$classMap = new Simec_SoapClient_ClassMap();
    	$className = get_class($this);
    	foreach (call_user_func(array(__CLASS__."Map", 'getClassMap')) as $type => $class) {
    		$classMap->add($type, $class);
    	}
    	return $classMap;
    }

    public function cadastrarPedidoAlteracao(PedidoAlteracaoDTO $pedido)
    {
        $cadPedidoAlteracao = new cadastrarPedidoAlteracao();
        $cadPedidoAlteracao->credencial = $this->credenciais;
        $cadPedidoAlteracao->pedidoAlteracao = $pedido;

        return $this->getSoapClient()->call('cadastrarPedidoAlteracao', array($cadPedidoAlteracao));
    }

    public function verificarPedidoAlteracao($siopid, $exercicio)
    {
        $verPedidoAlteracao = new verificarPedidoAlteracao();
        $verPedidoAlteracao->credencial = $this->credenciais;
        $verPedidoAlteracao->exercicio = $exercicio;
        $verPedidoAlteracao->identificadorUnico = $siopid;

        return $this->getSoapClient()->call('verificarPedidoAlteracao', array($verPedidoAlteracao));
    }

    public function obterPerguntasJustificativa()
    {
        $obtPerguntasDeJustificativa = new obterPerguntasJustificativa();
        $obtPerguntasDeJustificativa->credencial = $this->credenciais;
        $obtPerguntasDeJustificativa->orcamentoInvestimento = false;

        return $this->getSoapClient()->call('obterPerguntasJustificativa', array($obtPerguntasDeJustificativa));
    }


	public function obterEmendasAprovadas( $arrParr = array() )
   {
       $obterEmendasAprovadas = new obterEmendasAprovadas();
       $obterEmendasAprovadas->CredencialDTO = $this->credenciais;
       $obterEmendasAprovadas->FiltroEmendaAprovadaDTO = $this->getFiltroEmendaAprovadaDTO( $arrParr );

       return $this->getSoapClient()->call('obterEmendasAprovadas', array($obterEmendasAprovadas));
   }

   public function getFiltroEmendaAprovadaDTO( $arrParr = array() ){
	   	$ObjetoWS = new filtroEmendaAprovadaDTO();
	   	$ObjetoWS->exercicio = date('Y');
	   	if( $arrParr['codigoUO']  )
	   		$ObjetoWS->codigosUO = $this->getCodigosUO( $arrParr['codigoUO'] );
	   	if( $arrParr['codigoParlamentar']  )
		   	$ObjetoWS->codigosParlamentares = $this->getCodigosParlamentares( $arrParr['codigoParlamentar'] );

	   	return $ObjetoWS;
   }

   public function getCodigosParlamentares( $codigoParlamentar ){
	   	$ObjetoWS = new codigosParlamentares();
	   	$ObjetoWS->codigoParlamentar = $codigoParlamentar;

	   	return $ObjetoWS;
   }

   public function getCodigosUO( $codigoUO ){
	   	$ObjetoWS = new CodigosUO();
	   	$ObjetoWS->codigoUO = $codigoUO;

	   	return $ObjetoWS;
   }

    public function obterTabelasApoioAlteracoesOrcamentarias($exercicio)
    {
        $obtTabelasApoio = new obterTabelasApoioAlteracoesOrcamentarias();
        $obtTabelasApoio->credencial = $this->credenciais;
        $obtTabelasApoio->exercicio = $exercicio;
        $obtTabelasApoio->retornarTiposFonteRecurso = true;
        $obtTabelasApoio->retornarTiposAlteracao = false;

        return $this->getSoapClient()->call('obterTabelasApoioAlteracoesOrcamentarias', array($obtTabelasApoio));
    }

    function excluirPedidoAlteracao($identificadorUnicoPedido, $exercicio)
    {
        $excluirPedidoAlteracao = new ExcluirPedidoAlteracao();
        $excluirPedidoAlteracao->credencial = $this->credenciais;
        $excluirPedidoAlteracao->exercicio = $exercicio;
        $excluirPedidoAlteracao->identificadorUnico = $identificadorUnicoPedido;

        return $this->getSoapClient()->call('excluirPedidoAlteracao', array($excluirPedidoAlteracao));
    }

    function obterPedidosAlteracao($identificadorUnicoPedido, $exercicio, $momento = null)
    {
        $obterPedidoAlteracao = new ObterPedidoAlteracao();
        $obterPedidoAlteracao->credencial = $this->credenciais;
        $obterPedidoAlteracao->exercicio = $exercicio;
        $obterPedidoAlteracao->identificadorUnico = $identificadorUnicoPedido;
        $obterPedidoAlteracao->codigoMomento = $momento;

        return $this->getSoapClient()->call('obterPedidosAlteracao', array($obterPedidoAlteracao));
    }
}
