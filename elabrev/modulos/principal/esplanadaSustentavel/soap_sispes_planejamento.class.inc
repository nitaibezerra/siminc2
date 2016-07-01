<?php

include 'soap_sispes_planejamento_abstract.class.inc';


/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

class soapSispesPlanejamento 
{

    public function __construct($arrParam = array())
    {
        global $db;

        if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
            $urlWsdl = "https://hom.pes.planejamento.gov.br/sispes/MetaWSImpl?wsdl";
//            $this->certificado = APPRAIZ . "elabrev/modulos/sistema/comunica/mec.pem";
//            $this->senha_certificado = "mec2011";
        } else {
            $urlWsdl = "https://pes.planejamento.gov.br/sispes/MetaWSImpl?WSDL";
            /* $this->certificado 			=  "/etc/ssl/cert_simec_2012.pem";
              $this->senha_certificado 	= "12345678"; */
//            $this->certificado = APPRAIZ . "monitora/modulos/sistema/comunica/cert_simec_2012.pem";
//            $this->senha_certificado = "12345678";
        }

        if ($db) {
            $this->db = $db;
        } else {
            $this->db = new cls_banco();
        }

        $this->wsusuario = $arrParam['usuario'];
        $this->wssenha = $arrParam['senha'];
        $this->urlWsdl = $urlWsdl;
        $this->post = $arrParam['post'];
    }

    private function getCredencialDTO()
    {
        $credencialDTO = new credencialDTO();
        $credencialDTO->senha  	= '1QVC9RgaBRmm8SALwTI4m843xVbRKY';
        $credencialDTO->usuario	= 'MEC';

        return $credencialDTO;
    }
    
    private function getFinanceira($data){
        $obj = $this->objFinanceira();

        $obj->anoExercicio = ''; // int
        $obj->codigoUO = ''; // string
        $obj->listaMetasDespesa = $this->getMetaDespesaFinanceira($data); // metaDespesaFinanceiraDTO
        $obj->percentual = ''; // decimal
        $obj->valorMeta = '';

        return $obj;
    }
    
    public function getMetaFinanceira($data){
        $objMetaFinanceira = $this->objMetaFinanceira();
        
        $objMetaFinanceira->arg0 = $this->populate($this->getCredencial(), $data);
        $objMetaFinanceira->arg1 = $this->populate($this->getFinanceira($data), $data);

        return $objMetaFinanceira;
    }
    
    public function getMetaDespesaFinanceira($data)
    {
        $clsMetaDespesaFinanceiraDTO = $this->objDespesaFinanceira($data);
        $clsMetaDespesaFinanceiraDTO->tipoDespesa = '';
        $clsMetaDespesaFinanceiraDTO->valorMeta = '';

        return array($clsMetaDespesaFinanceiraDTO);
    }
    
    public function consultarTiposDespesaExercicioAtual()
    {
        $dto = $this->getConsultarTiposDespesaExercicioAtual();
        $wsQualitativo = new MetaWSImplService($this->urlWsdl, array(
//            'local_cert'	=> '', 
//            'passphrase ' 	=> '',
            'exceptions' => true,
            'trace' => true,
            'encoding' => 'ISO-8859-1')
        );
        
        echo "<pre>";
        var_dump($dto);
        
        return $wsQualitativo->consultarTiposDespesaExercicioAtual($dto);
    }
    
    public function getConsultarTiposDespesaExercicioAtual(){
        $consultarTiposDespesaExercicioAtual = new consultarTiposDespesaExercicioAtual();
        $consultarTiposDespesaExercicioAtual->arg0 = $this->getCredencialDTO();
        return $consultarTiposDespesaExercicioAtual;
    }


    public function metaFinanceira($data)
    {
        $data = $_POST;
        $objEnvio = $this->getMetaFinanceira($data);
        
        echo '<pre>';
        var_dump($objEnvio);
        exit;
        
        ver($objEnvio, d);
        $wsQualitativo = new MetaWSImplService($this->urlWsdl, array(
//            'local_cert'	=> $this->certificado, 
//            'passphrase ' 	=> $this->senha_certificado,
            'exceptions' => true,
            'trace' => true,
            'encoding' => 'ISO-8859-1')
        );
        $retorno = $wsQualitativo->inserirMetaFinanceira($objEnvio);
        return $retorno;
    }

    /**
     * Consulta todas as entidades de acordo com o codigo do orgao passado.
     * 
     * 
     */
    public function consultarEntidadesDoOrgao()
    {
        
        $params = new consultarEntidadesDoOrgao();
        $params->arg0 = $this->getCredencialDTO(); // CredencialDTO
        $params->arg1 = '26000'; // Codigo Orgao
        $params->arg2 = date('Y'); // Ano Exercicio 
        
        $wsQualitativo = new MetaWSImplService($this->urlWsdl, array(
//            'local_cert'	=> '', 
//            'passphrase ' 	=> '',
            'exceptions' => true,
            'trace' => true,
            'encoding' => 'ISO-8859-1')
        );
        
        return $wsQualitativo->consultarEntidadesDoOrgao($params); 
    }
    
    public function inserirMetaFinanceira($codigoUO , $percentual , $anoExercicio = null)
    {
        if(!$anoExercicio) $anoExercicio = date('Y');
        
        // Montando parametos para utilizar o servico de inserir os valores
        $inserirMetaFinanceira = new inserirMetaFinanceira();
        $inserirMetaFinanceira->arg0 = $this->getCredencialDTO();
        $inserirMetaFinanceira->arg1 = new metaFinanceiraDTO();
        $inserirMetaFinanceira->arg1->codigoUO     = $codigoUO;
        $inserirMetaFinanceira->arg1->percentual   = $percentual;
        $inserirMetaFinanceira->arg1->anoExercicio = $anoExercicio;
        
        $wsQualitativo = new MetaWSImplService($this->urlWsdl, array(
//            'local_cert'	=> '', 
//            'passphrase ' 	=> '',
            'exceptions' => true,
            'trace' => true,
            'encoding' => 'ISO-8859-1')
        );
        
        return $wsQualitativo->inserirMetaFinanceira($inserirMetaFinanceira); 
    }
    
    /**
     * 
     * 
     */
    public function inserirMetaEntidade($metaEntidadeDTOValues)
    {
        
        // Montando parametos para utilizar o servico de inserir o valor do tipo de despesas e metas anuais
        $inserirMetaEntidade = new inserirMetaEntidade();
        $inserirMetaEntidade->arg0 = $this->getCredencialDTO();
        $inserirMetaEntidade->arg1 = new metaEntidadeDTO();
        $inserirMetaEntidade->arg1->anoExercicio = $metaEntidadeDTOValues['anoExercicio'];
        $inserirMetaEntidade->arg1->codigoEntidade = $metaEntidadeDTOValues['codigoEntidade'];
        $inserirMetaEntidade->arg1->codigoUo = $metaEntidadeDTOValues['codigoUo'];
        
        foreach($metaEntidadeDTOValues['despesas'] as $key => $despesa ){
            $inserirMetaEntidade->arg1->despesas[$key] = new despesaEntidadeDTO();
            $inserirMetaEntidade->arg1->despesas[$key]->codigoDespesa = $despesa['codigoDespesa'];
            $inserirMetaEntidade->arg1->despesas[$key]->valor = $despesa['valor'];
        }
//        $inserirMetaEntidade->arg1->despesas = new despesaEntidadeDTO();
//        $inserirMetaEntidade->arg1->despesas->codigoDespesa = $metaEntidadeDTOValues['despesas']['codigoDespesa'];
//        $inserirMetaEntidade->arg1->despesas->valor = $metaEntidadeDTOValues['despesas']['valor'];
        
        $wsQualitativo = new MetaWSImplService($this->urlWsdl, array(
//            'local_cert'	=> '', 
//            'passphrase ' 	=> '',
            'exceptions' => true,
            'trace' => true,
            'encoding' => 'ISO-8859-1')
        );
        
        return $wsQualitativo->inserirMetaEntidade($inserirMetaEntidade); 
    }
    
//        public function metaFinanceira(){
//		
//		$obEnvio = $this->getMetaFinanceira();
//		ver($obEnvio,d);
//		$wsQualitativo = new MetaWSImplService($this->urlWsdl, array(
////															'local_cert'	=> $this->certificado, 
////															'passphrase ' 	=> $this->senha_certificado,
//															'exceptions'	=> true,
//													        'trace'			=> true,
//															'encoding'		=> 'ISO-8859-1' )
//											);
//		$retorno = $wsQualitativo->inserirMetaFinanceira( $obEnvio );
//		return $retorno;
//	}
        
        
//        public function getMetaFinanceira(){
//            $clsMetaFinanceira = new inserirMetaFinanceira();
//            
//            $clsMetaFinanceira->arg0 = $this->getCredencial();
//            $clsMetaFinanceira->arg1 = $this->getFinanceiraDTO();
//            
//            return $clsMetaFinanceira;
//        }
        
        public function getMetaDespesaFinanceiraDTO()
        {
            $clsMetaDespesaFinanceiraDTO = new metaDespesaFinanceiraDTO();
            $clsMetaDespesaFinanceiraDTO->tipoDespesa = '';
            $clsMetaDespesaFinanceiraDTO->valorMeta = '';
            
            return array($clsMetaDespesaFinanceiraDTO);
        }
        
        private function getFinanceiraDTO(){
            $clsMetaFinanceiraDTO = new metaFinanceiraDTO();
                    
            $clsMetaFinanceiraDTO->anoExercicio = ''; // int
            $clsMetaFinanceiraDTO->codigoUO = ''; // string
            $clsMetaFinanceiraDTO->listaMetasDespesa = $this->getMetaDespesaFinanceiraDTO(); // metaDespesaFinanceiraDTO
            $clsMetaFinanceiraDTO->percentual = ''; // decimal
            $clsMetaFinanceiraDTO->valorMeta = '';
                    
                    
                    
            
            return $clsMetaFinanceiraDTO;
        }
        
        
//        private function getCredencial(){
//		
//		$ObjetoWS = new credencialDTO();
////		$ObjetoWS->perfil 	= 32;
//		$ObjetoWS->senha  	= $this->wssenha;
//		$ObjetoWS->usuario	= $this->wsusuario;
//		
//		return $ObjetoWS;
//	}

}
