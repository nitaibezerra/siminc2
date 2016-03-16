<?php
	
class RemessaDetalhe extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.remessadetalhe";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rmdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	  'rmdid' => null, 
										  'rmcid' => null, 
										  'cs_tipo_registro' => null, 
										  'nu_seq_registro' => null, 
										  'cs_tipo_lote' => null, 
										  'nu_nib' => null, 
										  'dt_fim_periodo' => null, 
										  'dt_ini_periodo' => null, 
										  'cs_natur_credito' => null, 
										  'dt_mov_credito' => null, 
										  'id_orgao_pagador' => null, 
										  'vl_credito' => null,
    									  'cs_unid_monet' => null,	 
										  'dt_fim_validade' => null, 
										  'dt_ini_validade' => null, 
										  'in_cr_bloqueado' => null, 
										  'id_agencia_conv' => null, 
										  'in_prestacao_unica' => null, 
										  'nu_conta' => null, 
										  'cs_origem_orcamento' => null, 
										  'in_pioneira' => null, 
										  'cs_tipo_credito' => null, 
										  'nu_cpf' => null, 
										  'nm_beneficiario' => null, 
										  'te_endereco' => null, 
										  'te_bairro' => null, 
										  'nu_cep' => null, 
										  'dt_nasc' => null, 
										  'nm_mae' => null, 
										  'nu_dia_util' => null, 
										  'cs_tipo_dado_cad' => null, 
										  'nu_seq_credito' => null, 
										  'nu_ctrl_credito' => null, 
										  'dt_ult_atu_end' => null,
    									  'tutid' => null,);
    
    public function salvarDetalhe($rmcid,$nu_seq_lote)
    {
    	$arrTutid = $_POST['tutid'];
    	
    	$sql = "select * from maismedicos.tutor where tutid in (".implode(",",$arrTutid).") and tutstatus = 'A' and tutvalidade = true";
    	$arrTutores = $this->carregar($sql);
    	
    	$qtde_detalhe=0;
    	
    	foreach($arrTutores as $num => $t){
	    	/* Preenchimento dos campos de detalhe */
			  $arrDados['rmcid'] = $rmcid; //Id da tabela de maismedicos.cabecalho 
			  $arrDados['cs_tipo_registro'] = TRGID_DETALHE; //Dicionário de Dados: 1 para Header, 2 para Detalhe e 3 para Trailer. 
			  $arrDados['nu_seq_registro'] = preencherComZero(($num+2),6); 
			  $arrDados['cs_tipo_lote'] = TPLID_CADASTRO; //Dicionário de dados: 21 para cadastro e 20 para créditos
			  $arrDados['nu_nib'] = substr($t['tutcpf'], 0,9).modulo11(substr($t['tutcpf'], 0,9)); //CPF + Módulo 11
			  $arrDados['dt_fim_periodo'] = date("Ymd"); //Ver com Henrique 
			  $arrDados['dt_ini_periodo'] = date("Ymd"); //Ver com Henrique 
			  $arrDados['cs_natur_credito'] = ""; //Ver com Henrique 
			  $arrDados['dt_mov_credito'] = date("Ymd"); //Data atual
			  //Início - Agência bancária sem o dígito
			  $arrAg = explode("-",$t['tutagencia']);
			  $agencia =$arrAg[0];
			  $agencia = removeCaracteres($agencia,array(".","-","/"));
			  $arrDados['id_orgao_pagador'] = preencherComZero(substr($agencia, 0,6),6);
			  //Início - Agência bancária sem o dígito 12.400-1
			  $arrDados['vl_credito'] = null; //Ver com Henrique 
			  $arrDados['dt_fim_validade'] = ""; //Ver com Henrique  
			  $arrDados['dt_ini_validade'] = ""; //Ver com Henrique 
			  $arrDados['in_cr_bloqueado'] = ""; 
			  $arrDados['id_agencia_conv'] = preencherComZero("1607",8); //Agência fixa: 1607
			  $arrDados['in_prestacao_unica'] = "1"; //ID Fixo: 1
			  $arrDados['nu_conta'] = ""; //Em branco
			  $arrDados['in_pioneira'] = ""; //0 Não Pioneira ou 1 Pioneira Ver com Henrique
			  $arrDados['cs_tipo_credito'] = "03"; //ID Fixo 3 
			  $arrDados['nu_cpf'] = $t['tutcpf']; // CPF do Tutor 
			  $arrDados['nm_beneficiario'] = substr(str_to_upper(removeAcentosRemessa($t['tutnome'])), 0,28); //Nome do beneficiário, apenas 28 cacarteres
			  $arrDados['te_endereco'] = substr(str_to_upper(removeAcentosRemessa($t['tutlogradouro'])), 0,40); //Endereço do beneficiário, apenas 40 cacarteres
			  $arrDados['te_bairro'] = substr(str_to_upper(removeAcentosRemessa($t['tutbairro'])), 0,17); //Bairro do beneficiário, apenas 17 cacarteres 
			  $arrDados['nu_cep'] = $t['tutcep']; //CEP do beneficiário
			  //Início - Data Nascimento
			  $data_nasc = $t['tutdatanascimento'];
			  $arrData = explode(" ",$data_nasc);
			  $data_nascimento = $arrData[0];
			  $arrDados['dt_nasc'] = removeCaracteres($data_nascimento,array(".","-","/"));
			  //Fim - Data Nascimento
			  $arrDados['nm_mae'] = substr(str_to_upper(removeAcentosRemessa($t['tutnomemae'])), 0,32); //Nome da Mãe, 32 caracteres 
			  $arrDados['nu_dia_util'] = "10"; //ID Fixo: 10 
			  $arrDados['cs_tipo_dado_cad'] = "03"; //ID Fixo: 03 
			  $arrDados['nu_seq_credito'] = ""; //Ver com Henrique
			  $arrDados['nu_ctrl_credito'] = ""; //Ver com Henrique
			  //Início - Data Atualização
			  $data_atu = $t['tutdataatualizacao'];
			  $arrDataAtu = explode(" ",$data_atu);
			  $data_atualizacao = $arrDataAtu[0];
			  $arrDados['dt_ult_atu_end'] = removeCaracteres($data_atualizacao,array(".","-","/"));
			  //Fim - Data Atualização
			  $arrDados['fpgid'] = ""; //Ver com Henrique
			  $arrDados['tutid'] = $t['tutid']; //ID da tabela de tutores
			  $n = new RemessaDetalhe();
			  $n->popularDadosObjeto($arrDados);
			  $n->salvar();
			  $n->commit();
			  $qtde_detalhe++;
    	}
    	
    	$rodape = new RemessaRodape();
    	$rodape->salvarRodape($rmcid,$qtde_detalhe,$nu_seq_lote);
    	
    }
    
    public function preparaTxtRemessaCadastroDetalhe($rmcid)
    {
    	$arrDetalhe = $this->recuperarTodos("*",array("rmcid = $rmcid"));
    	if($arrDetalhe){
    		foreach($arrDetalhe as $d){
    			$txt.= $d['cs_tipo_registro'];
    			$txt.= preencherComZero($d['nu_seq_registro'],7);
				//Início Tipo de Lote
				$tipo_lote = new TipoLote($d['cs_tipo_lote']); 
				$txt.= $tipo_lote->tplcod;	
				//Fim Tipo de Lote
				$txt.= $d['nu_nib'];
				$txt.= $d['id_orgao_pagador'];
				$txt.= preencherComZero("0", 3); //CS Espécie, tamanho 3
				$txt.= preencherComZero($d['id_agencia_conv'],8);
				$txt.= $d['in_prestacao_unica'];
				$txt.= preencherComZero("0", 10); //Nu Conta, tamanho 10
				$txt.= $d['nu_cpf'];
				$txt.= preencherComEspacoVazio(" ", 13); //Filler, tamanho 13, em branco
				$txt.= preencherComEspacoVazio($d['nm_beneficiario'], 28,true);
				$txt.= preencherComZero("0", 11); //ID-NIT, tamanho 11
				$txt.= preencherComEspacoVazio($d['te_endereco'], 40,true);
				$txt.= preencherComEspacoVazio($d['te_bairro'], 17,true);
				$txt.= preencherComEspacoVazio($d['nu_cep'], 8,true);
				$txt.= $d['dt_nasc'];
				$txt.= preencherComEspacoVazio($d['nm_mae'], 32,true);
				$txt.= $d['nu_dia_util'];
				$txt.= $d['cs_tipo_dado_cad'];
				$txt.= $d['dt_ult_atu_end'];
				$txt.= preencherComEspacoVazio(" ", 12); //Filler, tamanho 13, em branco
				$txt.= "\n";
    		}
    	}
    	
    	$rodape = new RemessaRodape();
    	$txt.= $rodape->preparaTxtRemessaCadastroRodape($rmcid,count($arrDetalhe));
    	
    	return $txt;
    }
    
    public function salvarDetalheCredito($rmcid,$nu_seq_lote,$fpgid)
    {
    	$arrTutid = $_POST['tutid'];
    	 
    	$sql = "select 
    				* 
    			from 
    				maismedicos.tutor tut 
    			inner join 
    				maismedicos.autorizacaopagamento apg ON apg.apgcpf = tut.tutcpf and apg.apgtipo = tut.tuttipo
    			where
    				tutid in (".implode(",",$arrTutid).")
    			and 
    				apgstatus = 'A'
    			and
					apg.rmdid is null";
    	
    	$arrTutores = $this->carregar($sql);
    	 
    	$qtde_detalhe=0;
    	
    	$num_nu_seq_registro = 2;
    	 
    	foreach($arrTutores as $num => $t){
    		/* Preenchimento dos campos de detalhe */
    		$arrDados['rmcid'] = $rmcid; //Id da tabela de maismedicos.cabecalho
    		$arrDados['cs_tipo_registro'] = TRGID_DETALHE; //Dicionário de Dados: 1 para Header, 2 para Detalhe e 3 para Trailer.
    		$arrDados['nu_seq_registro'] = preencherComZero(($num_nu_seq_registro),6);
    		$arrDados['cs_tipo_lote'] = TPLID_CREDITO; //Dicionário de dados: 21 para cadastro e 20 para créditos
    		$arrDados['nu_nib'] = substr($t['tutcpf'], 0,9).modulo11(substr($t['tutcpf'], 0,9)); //CPF + Módulo 11
    		
//     		$arrDados['dt_ini_periodo'] = date('Y', strtotime("-30 days")).date('m', strtotime("-30 days"))."01"; //01 do mês anterior
    		$arrDados['dt_ini_periodo'] = date($t['apgano'].$t['apgmes'].'01');
    		
    		$arr_meses_com_31_dias = array("01","03","05","07","08","10","12");
    		if(in_array(date('m', strtotime("-30 days")),$arr_meses_com_31_dias)){
    			$dia = "31";
    		}else{
    			$dia = "30";
    		}
    		if(date('m', strtotime("-30 days")) == "02")
    		{
    			$dia = "29";
    		}
    		
//     		$arrDados['dt_fim_periodo'] = date('Y', strtotime("-30 days")).date('m', strtotime("-30 days"))."$dia"; //31 ou 30 do mês anterior
			$dias = cal_days_in_month(CAL_GREGORIAN, $t['apgmes'], $t['apgano']);
    		$arrDados['dt_fim_periodo'] = date($t['apgano'].$t['apgmes'].$dias);
    		
    		$arrDados['cs_natur_credito'] = "01"; //Ver com Henrique
    		$arrDados['dt_mov_credito'] = date("Ym")."20"; //Dia 20 do mês corrente
    		//Início - Agência bancária sem o dígito
    		$arrAg = explode("-",$t['tutagencia']);
    		$agencia =$arrAg[0];
    		$agencia = removeCaracteres($agencia,array(".","-","/"));
    		$arrDados['id_orgao_pagador'] = preencherComZero(substr($agencia, 0,6),6);
    		//5k Tutor e 4k supervisor
    		if($t['apgtipo'] == "T"){
    			$arrDados['vl_credito'] = "5000"; //5.000,00
    		}else{
    			$arrDados['vl_credito'] = "4000"; //4.000,00
    		}
    		$vl_reg_detalhe+=$arrDados['vl_credito'];
    		$arrDados['cs_unid_monet'] = "8"; //Fixo: 8 (R$ Real)
    		//$arrDados['dt_ini_validade'] = date("Ym")."10"; //Ver com Henrique
    		$arrDados['dt_ini_validade'] = date("Ymd"); //Ver com Henrique
    		//$arrDados['dt_fim_validade'] = date('Ymd', strtotime("+90 days",strtotime(date("Y").date("m")."10")));; //Ver com Henrique //90 dias a partir do 10 dia do mês corrente
    		$arrDados['dt_fim_validade'] = date('Ymd', strtotime("+90 days",strtotime(date("Y").date("m").date("d")))); //Ver com Henrique //90 dias a partir do 10 dia do mês corrente
    		$arrDados['in_cr_bloqueado'] = "1"; //Ver com Henrique
    		$arrDados['id_agencia_conv'] = preencherComZero("1607",8); //Agência fixa: 1607
    		$arrDados['in_prestacao_unica'] = "1"; //ID Fixo: 1
    		$arrDados['nu_conta'] = ""; //Em branco
    		$arrDados['cs_origem_orcamento'] = "01"; //Fixo: 01
    		$arrDados['in_pioneira'] = "0"; //0 Não Pioneira ou 1 Pioneira Ver com Henrique
    		$arrDados['cs_tipo_credito'] = "03"; //ID Fixo 03
    		$arrDados['nu_cpf'] = $t['tutcpf']; // CPF do Tutor
    		$arrDados['nu_seq_credito'] = ""; //Inserir o ID do detalhe
    		$arrDados['nu_ctrl_credito'] = preencherComZero($fpgid,7); //Inserir o ID da folha
    		$arrDados['tutid'] = $t['tutid']; //ID da tabela de tutores
    		$n = new RemessaDetalhe();
    		$n->popularDadosObjeto($arrDados);
    		$rmdid = $n->salvar();
    		$n->commit();
    		$qtde_detalhe++;
    		$num_nu_seq_registro++;
    		
	    	//Atualiza o campo nu_seq_credito com o ID do detalhe inserido
// 	    	$sql = "update maismedicos.remessadetalhe set nu_seq_credito = rmdid where rmcid = $rmcid;
// 	    			update maismedicos.autorizacaopagamento set rmdid = $rmdid where apgcpf = '{$t['tutcpf']}' and apgmes = '".date('m', strtotime("-30 days"))."' and apgano = '".date('Y', strtotime("-30 days"))."'";

    		$sql = "update maismedicos.remessadetalhe set nu_seq_credito = rmdid where rmcid = $rmcid;
    				update maismedicos.autorizacaopagamento set rmdid = $rmdid where apgcpf = '{$t['tutcpf']}' and apgmes = '{$t['apgmes']}' and apgano = '{$t['apgano']}' and apgstatus = 'A' and rmdid is null";
	    		    	
	    	$this->executar($sql);
	    	$this->commit();
    	}
    	
    	$rodape = new RemessaRodape();
    	$rodape->salvarRodapeCredito($rmcid,$qtde_detalhe,$nu_seq_lote,$vl_reg_detalhe);
    }
    
    public function preparaTxtRemessaCreditoDetalhe($rmcid)
    {
    	$arrDetalhe = $this->recuperarTodos("*",array("rmcid = $rmcid"),"nu_seq_registro");
    	if($arrDetalhe){
    		foreach($arrDetalhe as $d){
    			$txt.= $d['cs_tipo_registro'];
    			$txt.= preencherComZero($d['nu_seq_registro'],7);
    			//Início Tipo de Lote
    			$tipo_lote = new TipoLote($d['cs_tipo_lote']);
    			$txt.= $tipo_lote->tplcod;
    			//Fim Tipo de Lote
    			$txt.= $d['nu_nib'];
     			$txt.= $d['dt_fim_periodo'];
    			$txt.= $d['dt_ini_periodo'];
    			$txt.= $d['cs_natur_credito'];
    			$txt.= $d['dt_mov_credito'];
    			$txt.= $d['id_orgao_pagador'];
    			$txt.= preencherComZero(str_replace(array(".",","),array("",""),number_format($d['vl_credito'],2,',','.')), 12);
    			$txt.= $d['cs_unid_monet'];
    			$txt.= $d['dt_fim_validade'];
    			$txt.= $d['dt_ini_validade'];
    			$txt.= $d['in_cr_bloqueado'];
    			$txt.= preencherComZero($d['nu_conta'],10);
    			$txt.= $d['cs_origem_orcamento'];
    			$txt.= $d['in_pioneira'];
    			$txt.= preencherComEspacoVazio(" ", 5); //Filler, tamanho 5, em branco
    			$txt.= preencherComZero("0", 11); //ID-NIT, tamanho 11
    			$txt.= preencherComZero($d['cs_tipo_credito'],2);
    			$txt.= preencherComZero("0", 3); //CS-ESPECIE, tamanho 3, zeros
    			$txt.= preencherComZero("0", 1); //Filler, tamanho 1, 0
    			$txt.= preencherComZero($d['nu_seq_credito'], 11);
    			$txt.= preencherComEspacoVazio(" ", 105); //Filler, tamanho 105, em branco
    			$txt.= preencherComZero($d['nu_ctrl_credito'], 7);
     			$txt.= "\n";
    		}
    	}
    	 
    	$rodape = new RemessaRodape();
    	$txt.= $rodape->preparaTxtRemessaCreditoRodape($rmcid,count($arrDetalhe));
    	return $txt;
    }
    
}