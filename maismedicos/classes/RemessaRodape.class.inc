<?php
	
class RemessaRodape extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.remessarodape";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rmrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	  'rmrid' => null,
										  'rmcid' => null,
										  'cs_tipo_registro' => null,
										  'nu_seq_registro' => null,
										  'cs_tipo_lote' => null,
										  'id_banco' => null,
										  'qt_reg_dtalhe' => null,
										  'vl_reg_detalhe' => null,
										  'nu_seq_lote' => null,);
  
   public function salvarRodape($rmcid,$qtdeDetalhe,$nu_seq_lote)
   {
   	
   	/* Preenchimento dos campos de rodapé */
	  $arrDados['rmcid'] = $rmcid;
	  $arrDados['cs_tipo_registro'] = TRGID_TRAILER; //Dicionário de Dados: 1 para Header, 2 para Detalhe e 3 para Trailer.
	  $arrDados['nu_seq_registro'] = preencherComZero($qtdeDetalhe+2,7); //Sequencial para cada linha do lote, tamanho 7
	  $arrDados['cs_tipo_lote'] = TPLID_CADASTRO; //Dicionário de dados: 21 para cadastro e 20 para créditos
	  $arrDados['id_banco'] = "001"; //ID Fixo: 001
	  $arrDados['qt_reg_dtalhe'] =  preencherComZero($qtdeDetalhe,8);
	  $arrDados['vl_reg_detalhe'] = ""; //Ver com Henrique
	  $arrDados['nu_seq_lote'] = $nu_seq_lote; //Número sequencial, incrementado quando mais de um lote for enviado na mesma data
   	
   	$this->popularDadosObjeto($arrDados);
   	$pk = $this->salvar();
   	$this->commit();
   	return $pk;
   }
   
   public function preparaTxtRemessaCadastroRodape($rmcid,$qtdeDetalhe)
   {
   	$sql = "select * from maismedicos.remessarodape where rmcid = $rmcid";
   	$arrDados = $this->pegaLinha($sql);
   	
   	$txt = $arrDados['cs_tipo_registro'];
   	$txt.= $arrDados['nu_seq_registro'];
   	//Início Tipo de Lote
   	$tipo_lote = new TipoLote($arrDados['cs_tipo_lote']);
   	$txt.= $tipo_lote->tplcod;
   	//Fim Tipo de Lote
   	$txt.= preencherComZero($arrDados['id_banco'],3);
   	$txt.= $arrDados['qt_reg_dtalhe'];
   	$txt.= $arrDados['nu_seq_lote'];
   	$txt.= preencherComEspacoVazio(" ",217); //Filler
   	//dbg(strlen($txt));
   	//dbg($txt,1);
   	return $txt;
   }
   
   function salvarRodapeCredito($rmcid,$qtdeDetalhe,$nu_seq_lote,$vl_reg_detalhe)
   {
	   	/* Preenchimento dos campos de rodapé */
	   	$arrDados['rmcid'] = $rmcid;
	   	$arrDados['cs_tipo_registro'] = TRGID_TRAILER; //Dicionário de Dados: 1 para Header, 2 para Detalhe e 3 para Trailer.
	   	$arrDados['nu_seq_registro'] = preencherComZero($qtdeDetalhe+2,7); //Sequencial para cada linha do lote, tamanho 7
	   	$arrDados['cs_tipo_lote'] = TPLID_CREDITO; //Dicionário de dados: 21 para cadastro e 20 para créditos
	   	$arrDados['id_banco'] = "001"; //ID Fixo: 001
	   	$arrDados['qt_reg_dtalhe'] =  preencherComZero($qtdeDetalhe,8);
	   	$arrDados['vl_reg_detalhe'] = preencherComZero(str_replace(array(".",","),array("",""),number_format($vl_reg_detalhe,2,',','.')),17); //Somatório dos valores do detalhe
	   	$arrDados['nu_seq_lote'] = $nu_seq_lote; //Número sequencial, incrementado quando mais de um lote for enviado na mesma data
	   	
	   	$this->popularDadosObjeto($arrDados);
	   	$pk = $this->salvar();
	   	$this->commit();
	   	return $pk;
   }
   
   function preparaTxtRemessaCreditoRodape($rmcid,$qtdeDetalhe)
   {
   	$sql = "select * from maismedicos.remessarodape where rmcid = $rmcid";
   	$arrDados = $this->pegaLinha($sql);
   	
   	$txt = $arrDados['cs_tipo_registro'];
   	$txt.= $arrDados['nu_seq_registro'];
   	//Início Tipo de Lote
   	$tipo_lote = new TipoLote($arrDados['cs_tipo_lote']);
   	$txt.= $tipo_lote->tplcod;
   	//Fim Tipo de Lote
   	$txt.= preencherComZero($arrDados['id_banco'],3);
   	$txt.= $arrDados['qt_reg_dtalhe'];
   	$txt.= $arrDados['vl_reg_detalhe'];
   	$txt.= $arrDados['nu_seq_lote'];
   	$txt.= preencherComEspacoVazio(" ",200); //Filler
   	//dbg(strlen($txt));
   	//dbg($txt,1);
   	return $txt;
   }
    
}