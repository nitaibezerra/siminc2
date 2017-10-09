<?php
	
class RemessaCabecalho extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.remessacabecalho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rmcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rmcid' => null, 
									  	'cs_tipo_registro' => null,
    									'nu_seq_registro' => null,
							    		'cs_tipo_lote' => null,
							    		'dt_hora' => null,
							    		'id_banco' => null,
							    		'cs_meio_pagto' => null,
							    		'dt_gravacao_lote' => null,
							    		'nu_seq_lote' => null,
							    		'nu_cod_conv' => null,
							    		'nu_ctrl_trans' => null,
    									'fpgid' => null,);
     
   public function processarCadastro()
   {
   	
   	/* Preenchimento dos campos de cabeçalho */
   	$arrDado['cs_tipo_registro'] = TRGID_CABECALHO; //Dicionário de Dados: 1 para Header, 2 para Detalhe e 3 para Trailer.
   	$arrDado['nu_seq_registro'] = "0000001"; //Sequencial para cada envio de lote, cabeçalho recebe sempre 0000001
   	$arrDado['cs_tipo_lote'] = TPLID_CADASTRO; //Dicionário de dados: 21 para cadastro e 20 para créditos
   	//$arrDado['dt_hora'] = "now()"; //Padrão do SIMEC, automático no banco
   	$arrDado['id_banco'] = "001"; //ID Fixo: 001
   	$arrDado['cs_meio_pagto'] = "01"; //ID Fixo: 01
   	$arrDado['dt_gravacao_lote'] = date("Ymd"); //Data em que o arquivo foi gravado, no formato YYYYMMDD
   	$nu_seq_lote = verificaTransmissaoLoteDataAtual();
   	$arrDado['nu_seq_lote'] = preencherComZero(($nu_seq_lote),2); //Número sequencial, incrementado quando mais de um lote for enviado na mesma data
   	$arrDado['nu_cod_conv'] = "000526"; //ID Fixo: 000526
   	//Início - Número incrementado a cada transmissão de lote (serial)
   	$sql = "select count(rmcid) from $this->stNomeTabela where cs_tipo_lote = 2";
   	$nu_ctrl_trans = $this->pegaUm($sql);
   	$nu_ctrl_trans = $nu_ctrl_trans ? $nu_ctrl_trans : "0";
   	$nu_ctrl_trans = preencherComZero(($nu_ctrl_trans+1),6);
   	$arrDado['nu_ctrl_trans'] = $nu_ctrl_trans; 
   	//Fim - Número incrementado a cada transmissão de lote (serial)
   	
   	$this->popularDadosObjeto($arrDado);
   	$rmcid = $this->salvar();
   	if($rmcid){
   		
   		$detalhe = new RemessaDetalhe();
   		$detalhe->salvarDetalhe($rmcid,$arrDado['nu_seq_lote']);
   		
   		$this->commit();
   		$_SESSION['maismedicos']['remessa']['alert'] = "Operação realizada com sucesso.";
   	}else{
   		$_SESSION['maismedicos']['remessa']['alert'] = "Não foi possível realizar a operação.";
   	}
   	
   	header("Location: maismedicos.php?modulo=principal/remessaCadastro&acao=A");
   	exit;
   }
   
   public function gerarArquivoRemessaCadastro($rmcid)
   {

   	$txt = $this->preparaTxtRemessaCadastroCabecalho($rmcid);
   	
   	$detalhe = new RemessaDetalhe();
   	$txt.= $detalhe->preparaTxtRemessaCadastroDetalhe($rmcid);
   	   	
   	if(!is_dir(APPRAIZ . 'arquivos/maismedicos')) {
   		mkdir(APPRAIZ . 'arquivos/maismedicos', 0777);
   	}
   	 
   	if(!is_dir(APPRAIZ . 'arquivos/maismedicos/remessacadastro')) {
   		mkdir(APPRAIZ . 'arquivos/maismedicos/remessacadastro', 0777);
   	}
   	 
   	$caminho = APPRAIZ . 'arquivos/maismedicos/remessacadastro/'.$rmcid.'.txt';
   	
   	$fp = fopen($caminho, "w+");
   	
   	stream_set_write_buffer($fp, 0);
   	fwrite($fp, $txt);
   	fclose($fp);
   	
   	$filename = $caminho;
   	header( 'Content-type: text/plain' );
   	header( 'Content-Disposition: attachment; filename=remessa_'.$rmcid);
   	readfile( $caminho );
   	exit();
   	
   }
   
   public function preparaTxtRemessaCadastroCabecalho($rmcid)
   {
	$txt = $this->arAtributos['cs_tipo_registro'];	
	$txt.= $this->arAtributos['nu_seq_registro'];	
	//Início Tipo de Lote
	$tipo_lote = new TipoLote($this->arAtributos['cs_tipo_lote']); 
	$txt.= $tipo_lote->tplcod;	
	//Fim Tipo de Lote
	$txt.= preencherComZero($this->arAtributos['id_banco'],3);
	$txt.= preencherComZero($this->arAtributos['cs_meio_pagto'],2);
	$txt.= $this->arAtributos['dt_gravacao_lote'];
	$txt.= $this->arAtributos['nu_seq_lote'];
	$txt.= preencherComEspacoVazio(" ",7); //Filler
	$txt.= $this->arAtributos['nu_cod_conv'];
	$txt.= preencherComEspacoVazio(" ",56); //Filler
	$txt.= $this->arAtributos['nu_ctrl_trans'];
	$txt.= preencherComEspacoVazio(" ",140); //Filler
	$txt.= "\n";
	//dbg(strlen($txt));
	//dbg($txt,1);
   	return $txt;
   }
   
   public function validarRemessaCadastro()
   {
   	
   	$conteudo = file_get_contents($_FILES['arquivoremesssa']['tmp_name']);
   	$arrCabecalho['cs_tipo_registro'] = substr($conteudo, 0,1);
   	$arrCabecalho['nu_seq_registro'] = substr($conteudo, 1,7);
   	$arrCabecalho['cs_tipo_lote'] = substr($conteudo, 8,2);
   	$arrCabecalho['id_banco'] = substr($conteudo, 10,3);
   	$arrCabecalho['cs_meio_pagto'] = substr($conteudo, 13,2);
   	$arrCabecalho['dt_gravacao_lote'] = substr($conteudo, 15,8);
   	$arrCabecalho['nu_seq_lote'] = substr($conteudo, 23,2);
   	$arrCabecalho['filler_01'] = substr($conteudo, 25,7);
   	$arrCabecalho['nu_cod_conv'] = substr($conteudo, 31,6);
   	$arrCabecalho['filler_02'] = substr($conteudo, 38,56);
   	$arrCabecalho['nu_ctrl_trans'] = substr($conteudo, 94,6);
   	$arrCabecalho['filler_03'] = substr($conteudo, 100,140);
   	$cabecalho = substr($conteudo, 0,240);
   	$arrDetalhe = explode("\n",$conteudo);
   	if($arrDetalhe){
   		foreach($arrDetalhe as $linha => $det){
   			if($det && $linha != 0 && $linha != (count($arrDetalhe)-2) ){
   				$arrTutor = false;
   				$arrTutor['cs_tipo_registro'] = substr($det, 0,1);
   				$arrTutor['nu_seq_registro'] = substr($det, 2,6);
   				$arrTutor['cs_tipo_lote'] = substr($det, 8,2);
   				$arrTutor['nu_nib'] = substr($det, 10,10);
   				$arrTutor['id_orgao_pagador'] = substr($det, 20,6);
   				$arrTutor['cs_especie'] = substr($det, 26,3);
   				$arrTutor['id_agencia_conv'] = substr($det, 29,8);
   				$arrTutor['in_prestacao_unica'] = substr($det, 37,1);
   				$arrTutor['nu_conta'] = substr($det, 38,10);
   				$arrTutor['nu_cpf'] = substr($det, 48,11);
   				$arrTutor['filler_01'] = substr($det, 59,13);
   				$arrTutor['nm_beneficiario'] = substr($det, 72,28);
   				$arrTutor['id_nit'] = substr($det, 100,11);
   				$arrTutor['te_endereco'] = substr($det, 111,40);
   				$arrTutor['te_bairro'] = substr($det, 151,17);
   				$arrTutor['nu_cep'] = substr($det, 168,8);
   				$arrTutor['dt_nasc'] = substr($det, 176,8);
   				$arrTutor['nm_mae'] = substr($det, 184,32);
   				$arrTutor['nu_dia_util'] = substr($det, 216,2);
   				$arrTutor['cs_tipo_dado_cad'] = substr($det, 218,2);
   				$arrTutor['dt_ult_atu_end'] = substr($det, 220,8);
   				$arrTutor['nu_seq_rms'] = substr($det, 228,6);
   				$arrTutor['sit_cmd'] = substr($det, 234,4);
   				$arrTutor['sit_rms'] = substr($det, 238,1);
   				$arrTutor['filler_02'] = substr($det, 239,1);
   				$detalhe[] = $arrTutor;
   			}elseif($linha == (count($arrDetalhe)-2)) //Trailer (Rodapé)
   			{
   				$arrRodape['cs_tipo_registro'] = substr($det, 0,1);
   				$arrRodape['nu_seq_registro'] = substr($det, 1,7);
   				$arrRodape['cs_tipo_lote'] = substr($det, 8,2);
   				$arrRodape['id_banco'] = substr($det, 10,3);
   				$arrRodape['qt_reg_detalhe'] = substr($det, 13,8);
   				$arrRodape['nu_seq_lote'] = substr($det, 21,2);
   				$arrRodape['filler'] = substr($det, 23,217);
   			}
   		}
   	}

   	//Pesquisa qual é o registro que deve receber o arquivo de retorno
   	$sql = "select
   				rmcid
   			from
   				maismedicos.remessacabecalho
   			where
   				cs_tipo_registro = '{$arrCabecalho['cs_tipo_registro']}'
   			and
   				nu_seq_registro = '{$arrCabecalho['nu_seq_registro']}'
   			and
   				cs_tipo_lote = ".TPLID_CADASTRO."
   			and
   				id_banco = ".BNCID_BANCO_BRASIL."
   			and
   				cs_meio_pagto = '{$arrCabecalho['cs_meio_pagto']}'
   			--and
   				--dt_gravacao_lote = '{$arrCabecalho['dt_gravacao_lote']}'
   			and
   				nu_seq_lote = '{$arrCabecalho['nu_seq_lote']}'
   			and
   				nu_cod_conv = '{$arrCabecalho['nu_cod_conv']}'
   			and
   				nu_ctrl_trans = '{$arrCabecalho['nu_ctrl_trans']}'
   			";
   	//dbg($sql,1);
   	$rmcid = $this->pegaUm($sql);
   	//$rmcid = 70; //Remover esta linha !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   	if(!$rmcid){
   		$_SESSION['maismedicos']['remessa']['alert'] = "O arquivo de retorno não corresponde a nenhuma remessa.";
   		header("Location: maismedicos.php?modulo=principal/validarRemessaCadastro&acao=A");
   		exit;
   	}else{
   		$aqrid = $this->gravaRetornoRemessa();
   		if($aqrid){
   			$sql = "update maismedicos.remessacabecalho set arqid = $aqrid where rmcid = $rmcid";
   			$this->executar($sql);
   			$this->commit();
   		}else{
   			$_SESSION['maismedicos']['remessa']['alert'] = "Não foi possível fazer o upload do arquivo.";
   			header("Location: maismedicos.php?modulo=principal/validarRemessaCadastro&acao=A");
   			exit;
   		}
   		
   		if($detalhe){
   			$num_encontrados=0;
   			$num_nao_encontrados=0;
   			foreach($detalhe as $det){
   				$det['nm_beneficiario'] = trim($det['nm_beneficiario']);
   				$det['te_endereco'] = trim($det['te_endereco']);
   				$det['te_bairro'] = trim($det['te_bairro']);
   				$det['nm_mae'] = trim($det['nm_mae']);
   				$sql = "select 
   							rmdid
   						from
   							maismedicos.remessadetalhe
   						where
   							rmcid = $rmcid
   						and
   							cs_tipo_registro = '{$det['cs_tipo_registro']}'
   						and
   							nu_seq_registro = '{$det['nu_seq_registro']}'
   						and
   							cs_tipo_lote = ".TPLID_CADASTRO."
   						and
   							nu_nib = '{$det['nu_nib']}'
   						and
   							id_orgao_pagador = '{$det['id_orgao_pagador']}'
   						and
   							id_agencia_conv = '{$det['id_agencia_conv']}'
   						and
   							in_prestacao_unica = '{$det['in_prestacao_unica']}'
   						and
   							nu_cpf = '{$det['nu_cpf']}'
   						and
   							nm_beneficiario = '{$det['nm_beneficiario']}'
   						and
   							te_endereco = '{$det['te_endereco']}'
   						and
   							te_bairro = '{$det['te_bairro']}'
   						and
   							nu_cep = '{$det['nu_cep']}'
   						and
   							dt_nasc = '{$det['dt_nasc']}'
   						and
   							nm_mae = '{$det['nm_mae']}'
   						";
   				
   				$rmdid = $this->pegaUm($sql);
   				if($rmdid){
   					$num_encontrados++;
   					//Recupera o retorno
   					$sql = "select strid from maismedicos.situacaoregistro where strcod = '{$det['sit_cmd']}'";
   					$strid = $this->pegaUm($sql);
   					if($strid){
   						$sql = "update maismedicos.remessadetalhe set strid = $strid where rmdid = $rmdid";
   						$this->executar($sql);
   						$this->commit();
   					}
   				}else{
   					$num_nao_encontrados++;
   				}
   			}
   			
   			$_SESSION['maismedicos']['remessa']['alert'] = "Retorno: $num_encontrados atualizados. $num_nao_encontrados não atualizados.";
   			header("Location: maismedicos.php?modulo=principal/validarRemessaCadastro&acao=A");
   			exit;
   		}
   		
   	}
   }
   
   public function gravaRetornoRemessa($descricao = "Arquivo de Retorno da Remessa de Cadastro",$campo = "arquivoremesssa")
   {
   		
   		require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    	$file = new FilesSimec(null,null,"maismedicos");
    	$file->setUpload($descricao,$campo,false,false);
    	return $file->getIdArquivo();
   }
   
   public function listarDetalheRemessa($rmcid)
   {
	   	$sql = "select
	   				tut.tutnome,
	   				case when tut.tuttipo = 'T'
	   					then 'Tutor'
	   					else 'Supervisor'
	   				end as funcao,
	   				uni.uninome,
	   				coalesce(str.strdsc,'N/A') as strdsc
	   			from
	   				maismedicos.tutor tut
	   			inner join
	   				maismedicos.universidade uni ON uni.uniid = tut.uniid
	   			inner join
	   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid
	   			left join
	   				maismedicos.situacaoregistro str ON str.strid = det.strid
	   			where
	   				det.rmcid = $rmcid
	   			order by
	   				tut.tutnome";
	   	//dbg($sql,1);
	   	$arrCab = array("Nome","Função","Instituição","Situação");
	   	$this->monta_lista($sql, $arrCab, 100, 10, "N", "center", "");
   }
   
   public function listarDetalheRemessaCredito($rmcid, $excel = false)
   {
	   	$sql = "select
	   				tut.tutcpf,
	   				tut.tutnome,
	   				case when tut.tuttipo = 'T'
	   					then 'Tutor'
	   					else 'Supervisor'
	   				end as funcao,
	   				case when tut.tuttipo = 'T'
	   					then '5000'
	   					else '4000'
	   				end as valor,
	   				fpgdsc,
	   				uni.uninome,
	   				coalesce(sit.strdsc,'N/A') as tpodsc
	   			from
	   				maismedicos.tutor tut
	   			inner join
	   				maismedicos.universidade uni ON uni.uniid = tut.uniid
	   			inner join
	   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid
	   			inner join
	   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
	   			inner join
	   				maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
	   			left join
	   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia
	   			where
	   				det.rmcid = $rmcid
	   			order by
	   				tut.tutnome";
	   	//dbg($sql);
	   	$arrCab = array("CPF", "Nome","Função","Valor (R$)","Descrição","Instituição","Situação");
	   	if($excel){
	   		$this->monta_lista_tabulado($sql, $arrCab, 100000000, 100000);	
	   	}else{
	   		$this->monta_lista($sql, $arrCab, 100, 10, "S", "center", "");
	   	}
   }
   
   public function processarRemessaCredito()
   {
	   	/* Preenchimento dos campos de cabeçalho */
   		$folha = new FolhaPagamento();
   		$arrFolha['tfpid'] = TFPID_PAGAMENTO;
   		$arrFolha['fpgdsc'] = "Folha de Pagamento do Mês de ".retornaMesMaisMedicos($_POST['apgmes'])." de ".$_POST['apgano'];
   		$arrFolha['fpgmes'] = $_POST['apgmes'].$_POST['apgano'];
   		$arrFolha['usucpf'] = $_SESSION['usucpf'];
   		$folha->popularDadosObjeto($arrFolha);
   		$fpgid = $folha->salvar();
   		$folha->commit();
   		$aut = new AutorizacaoPagamento();
   		$aut->checkTutoresSelecionados($_POST['tutid']);
	   	$arrDado['fpgid'] = $fpgid; //Folha de Pagamento
	   	$arrDado['cs_tipo_registro'] = TRGID_CABECALHO; //Dicionário de Dados: 1 para Header, 2 para Detalhe e 3 para Trailer.
	   	$arrDado['nu_seq_registro'] = "0000001"; //Sequencial para cada envio de lote, cabeçalho recebe sempre 0000001
	   	$arrDado['cs_tipo_lote'] = TPLID_CREDITO; //Dicionário de dados: 21 para cadastro e 20 para créditos
	   	$arrDado['id_banco'] = "001"; //ID Fixo: 001
	   	$arrDado['cs_meio_pagto'] = "01"; //ID Fixo: 01
	   	$arrDado['dt_gravacao_lote'] = date("Ymd"); //Data em que o arquivo foi gravado, no formato YYYYMMDD
	   	$nu_seq_lote = verificaTransmissaoLoteDataAtual();
	   	$arrDado['nu_seq_lote'] = preencherComZero(($nu_seq_lote),2); //Número sequencial, incrementado quando mais de um lote for enviado na mesma data
	   	$arrDado['nu_cod_conv'] = "000526"; //ID Fixo: 000526
	   	//Início - Número incrementado a cada transmissão de lote (serial)
	   	$sql = "select count(rmcid)+3 from $this->stNomeTabela where cs_tipo_lote = 1"; //+ 3 pq a contagem já começou em 3.
	   	$nu_ctrl_trans = $this->pegaUm($sql);
	   	$nu_ctrl_trans = $nu_ctrl_trans ? $nu_ctrl_trans : "0";
	   	$nu_ctrl_trans = preencherComZero(($nu_ctrl_trans+1),6);
	   	$arrDado['nu_ctrl_trans'] = $nu_ctrl_trans;
	   	//Fim - Número incrementado a cada transmissão de lote (serial)
	   	
	   	$this->popularDadosObjeto($arrDado);
	   	$rmcid = $this->salvar();
	   	if($rmcid){
	   		 
	   		$detalhe = new RemessaDetalhe();
	   		$detalhe->salvarDetalheCredito($rmcid,$arrDado['nu_seq_lote'],$fpgid);
	   		 
	   		$this->commit();
	   		$_SESSION['maismedicos']['remessa']['alert'] = "Operação realizada com sucesso.";
	   	}else{
	   		$_SESSION['maismedicos']['remessa']['alert'] = "Não foi possível realizar a operação.";
	   	}
	   	
	   	header("Location: maismedicos.php?modulo=principal/remessaCredito&acao=A");
	   	exit;
   }
   
   function gerarArquivoRemessaCredito($rmcid)
   {
   	$txt = $this->preparaTxtRemessaCreditoCabecalho($rmcid);
   	   	
   	$detalhe = new RemessaDetalhe();
   	$txt.= $detalhe->preparaTxtRemessaCreditoDetalhe($rmcid);
   	
   	if(!is_dir(APPRAIZ . 'arquivos/maismedicos')) {
   		mkdir(APPRAIZ . 'arquivos/maismedicos', 0777);
   	}
   	 
   	if(!is_dir(APPRAIZ . 'arquivos/maismedicos/remessacredito')) {
   		mkdir(APPRAIZ . 'arquivos/maismedicos/remessacredito', 0777);
   	}
   	 
   	$caminho = APPRAIZ . 'arquivos/maismedicos/remessacredito/'.$rmcid.'.txt';
   	
   	$fp = fopen($caminho, "w+");
   	
   	stream_set_write_buffer($fp, 0);
   	fwrite($fp, $txt);
   	fclose($fp);
   	
   	$filename = $caminho;
   	header( 'Content-type: text/plain' );
   	header( 'Content-Disposition: attachment; filename=remessa_credito_'.$rmcid);
   	readfile( $caminho );
   	exit();
   }
   
   public function preparaTxtRemessaCreditoCabecalho($rmcid)
   {
   	$txt = $this->arAtributos['cs_tipo_registro'];
   	$txt.= $this->arAtributos['nu_seq_registro'];
   	//Início Tipo de Lote
   	$tipo_lote = new TipoLote($this->arAtributos['cs_tipo_lote']);
   	$txt.= $tipo_lote->tplcod;
   	//Fim Tipo de Lote
   	$txt.= preencherComZero($this->arAtributos['id_banco'],3);
   	$txt.= preencherComZero($this->arAtributos['cs_meio_pagto'],2);
   	$txt.= $this->arAtributos['dt_gravacao_lote'];
   	$txt.= $this->arAtributos['nu_seq_lote'];
   	$txt.= preencherComZero("0",7); //Filler
   	$txt.= $this->arAtributos['nu_cod_conv'];
   	$txt.= preencherComEspacoVazio(" ",56); //Filler
   	$txt.= $this->arAtributos['nu_ctrl_trans'];
   	$txt.= preencherComEspacoVazio(" ",140); //Filler
   	$txt.= "\n";
   	//dbg(strlen($txt));
   	//dbg($txt,1);
   	return $txt;
   }
   
   public function validarRemessaCredito()
   {
   	
   
   	$conteudo = file_get_contents($_FILES['arquivoremesssa']['tmp_name']);
   	$arrCabecalho['cs_tipo_registro'] = substr($conteudo, 0,1);
   	$arrCabecalho['nu_seq_registro'] = substr($conteudo, 1,7);
   	$arrCabecalho['cs_tipo_lote'] = substr($conteudo, 8,2);
   	$arrCabecalho['id_banco'] = substr($conteudo, 10,3);
   	$arrCabecalho['cs_meio_pagto'] = substr($conteudo, 13,2);
   	$arrCabecalho['dt_gravacao_lote'] = substr($conteudo, 15,8);
   	$arrCabecalho['nu_seq_lote'] = substr($conteudo, 23,2);
   	$arrCabecalho['dt_comp_movto'] = substr($conteudo, 25,6);
   	$arrCabecalho['nu_cod_conv'] = substr($conteudo, 31,6);
   	$arrCabecalho['filler_01'] = substr($conteudo, 37,57);
   	$arrCabecalho['nu_ctrl_trans'] = substr($conteudo, 94,6);
   	$arrCabecalho['filler_03'] = substr($conteudo, 100,140);
   	$cabecalho = substr($conteudo, 0,240);
   	
   	$arrDetalhe = explode("\n",$conteudo);
   	
   	if($arrDetalhe){
   		foreach($arrDetalhe as $linha => $det){
   			if($det && $linha != 0 && $linha != (count($arrDetalhe)-2) ){
   				$arrTutor = false;
   				$arrTutor['cs_tipo_registro'] = substr($det, 0,1);
   				$arrTutor['nu_seq_registro'] = substr($det,2,6);
   				$arrTutor['cs_tipo_lote'] = substr($det, 8,2);
   				$arrTutor['nu_nib'] = substr($det, 10,10);
   				$arrTutor['dt_fim_periodo'] = substr($det, 20,8);
   				$arrTutor['dt_ini_periodo'] = substr($det, 28,8);
   				$arrTutor['cs_natur_credito'] = substr($det, 36,2);
   				$arrTutor['dt_mov_credito'] = substr($det, 38,8);
   				$arrTutor['id_orgao_pagador'] = substr($det, 46,6);
   				$arrTutor['vl_credito'] = substr($det, 52,12);
   				$arrTutor['cs_unid_monet'] = substr($det, 64,1);
   				$arrTutor['dt_fim_validade'] = substr($det, 65,8);
   				$arrTutor['dt_ocorrencia'] = substr($det, 65,8);
   				$arrTutor['dt_ini_validade'] = substr($det, 73,8);
   				$arrTutor['in_cr_bloquado'] = substr($det, 81,1);
   				$arrTutor['nu_conta'] = substr($det, 82,10);
   				$arrTutor['cs_origem_rocamento'] = substr($det, 92,2);
   				$arrTutor['in_pioneira'] = substr($det, 94,1);
   				$arrTutor['filler_01'] = substr($det, 95,5);
   				$arrTutor['id_nit'] = substr($det, 100,11);
   				$arrTutor['cs_tipo_credito'] = substr($det, 111,2);
   				$arrTutor['cs_especie'] = substr($det, 113,3);
   				$arrTutor['nu_seq_rms'] = substr($det, 115,6);
   				$arrTutor['nu_seq_credito'] = (int)substr($det, 122,11);
   				$arrTutor['sit_cmd'] = substr($det, 133,4);
   				$arrTutor['cs_ocorrencia'] = substr($det, 133,4);
   				$arrTutor['sit_rms'] = substr($det, 137,1);
   				$arrTutor['filler_02'] = substr($det, 138,95);
   				$arrTutor['nu_ctrl_cred'] = substr($det, 233,7);
   				$detalhe[] = $arrTutor;
   			}elseif($linha == (count($arrDetalhe)-2)) //Trailer (Rodapé)
   			{
   				$arrRodape['cs_tipo_registro'] = substr($det, 0,1);
   				$arrRodape['nu_seq_registro'] = substr($det, 1,7);
   				$arrRodape['cs_tipo_lote'] = substr($det, 8,2);
   				$arrRodape['id_banco'] = substr($det, 10,3);
   				$arrRodape['qt_reg_detalhe'] = substr($det, 13,8);
   				$arrRodape['vl_reg_detalhe'] = substr($det, 21,17);
   				$arrRodape['nu_seq_lote'] = substr($det, 38,2);
   				$arrRodape['filler'] = substr($det, 40,200);
   			}
   		}
   	}
   
   	//Pesquisa qual é o registro que deve receber o arquivo de retorno
   	$sql = "select
   				rmcid
   			from
   				maismedicos.remessacabecalho
  		 	where
   				cs_tipo_registro = '{$arrCabecalho['cs_tipo_registro']}'
   			and
   				nu_seq_registro = '{$arrCabecalho['nu_seq_registro']}'
      		and
   				cs_tipo_lote = ".TPLID_CREDITO."
      		and
      			id_banco = ".BNCID_BANCO_BRASIL."
   			and
      			cs_meio_pagto = '{$arrCabecalho['cs_meio_pagto']}'
   			--and
   				--dt_gravacao_lote = '{$arrCabecalho['dt_gravacao_lote']}'
   			and
   				nu_seq_lote = '{$arrCabecalho['nu_seq_lote']}'
   			and
   				nu_cod_conv = '{$arrCabecalho['nu_cod_conv']}'
   			--and
   				--arqid is null
   			and
   				nu_ctrl_trans = '{$arrCabecalho['nu_ctrl_trans']}'
   		";
   	//dbg($sql,1);
   		$rmcid = $this->pegaUm($sql);
   		if(!$rmcid){
   			$_SESSION['maismedicos']['remessa']['alert'] = "O arquivo de retorno não corresponde a nenhuma remessa de crédito.";
   			header("Location: maismedicos.php?modulo=principal/validarRemessaCredito&acao=A");
      		exit;
	   	}else{
	   		$aqrid = $this->gravaRetornoRemessa("Arquivo de Retorno da Remessa de Crédito");
	   		if($aqrid){
	   			$sql = "update maismedicos.remessacabecalho set arqid = $aqrid where rmcid = $rmcid";
	   			$this->executar($sql);
	   			$this->commit();
	   		}else{
	   			$_SESSION['maismedicos']['remessa']['alert'] = "Não foi possível fazer o upload do arquivo.";
	   			header("Location: maismedicos.php?modulo=principal/validarRemessaCredito&acao=A");
	   			exit;
	   		}
   			 
	   		if($detalhe){
	   			$num_encontrados=0;
	   			$num_nao_encontrados=0;
	   			foreach($detalhe as $det){
	   				$sql = "select
		   						rmdid
		   					from
		   						maismedicos.remessadetalhe	
		   					where
		   						rmcid = $rmcid
		   					and
		   						cs_tipo_registro = '{$det['cs_tipo_registro']}'
		   					and
		   						nu_seq_registro = '{$det['nu_seq_registro']}'
		   					and
		   						nu_seq_credito = '{$det['nu_seq_credito']}'
		   					and
		   						cs_tipo_lote = ".TPLID_CREDITO."
		   					and
		   						nu_nib = '{$det['nu_nib']}'
		   					and
		   						id_orgao_pagador = '{$det['id_orgao_pagador']}'
	      				";
      					$rmdid = $this->pegaUm($sql);
      					if($rmdid){
      						$num_encontrados++;
      						
      						$sql = "update 
      									maismedicos.remessadetalhe 
      								set 
      									cs_ocorrencia	= '{$det['cs_ocorrencia']}', 
      									id_nit 			= '{$det['id_nit']}', 
      									cs_especie 		= '{$det['cs_especie']}', 
      									nu_seq_rms 		= '{$det['nu_seq_rms']}', 
      									sit_cmd			= '{$det['sit_cmd']}', 
      									sit_rms 		= '{$det['sit_rms']}', 
      									nu_ctrl_cred	= '{$det['nu_ctrl_cred']}', 
      									dt_ocorrencia 	= '{$det['dt_ocorrencia']}' 
      								where 
      									rmdid = $rmdid";
      						$this->executar($sql);
      						
      						if($this->commit() && $det['cs_ocorrencia'] == '0000'){
      							$this->enviaEmailValidarRemessaCredito($rmdid, true);
      						}
      						
      					}else{
      						$num_nao_encontrados++;
      					}
      				}
      				$_SESSION['maismedicos']['remessa']['alert'] = "Retorno: $num_encontrados atualizados. $num_nao_encontrados não atualizados.";
      				header("Location: maismedicos.php?modulo=principal/validarRemessaCredito&acao=A");
      				exit;
	   			}
   			 
   			}
   		}
   		
   		public function conteudoEmailValidarRemessaCredito($arDados = array())
   		{
   			$txt = "<p>Prezado(a)&nbsp;<b>{$arDados['nome']}</b>,</p>
   					
					<p>Informamos que sua bolsa de {$arDados['tipobolsa']} do Projeto Mais Médicos para o Brasil, referente ao exercício de {$arDados['mesano']}, 
					no valor de R$ {$arDados['valor']}. {$arDados['vlrextenso']}, já está disponível para saque, na conta específica do benefício.</p>
					
					<p>Favor dirigir-se à agência do Banco do Brasil nº {$arDados['agbanco']}, informada no cadastro, para pegar seu Cartão de Pagamento de 
					Benefício (consulte o endereço da agência <a href=\"http://www37.bb.com.br/portalbb/redeAtendimento/Inicio,2,2316,2316.bbx\" target=\"_blank\">aqui</a> link para a consulta de agências BB).</p>
					 
					<p>Para resgatar o cartão e cadastrar senha, é indispensável a apresentação do documento de identificação cadastrado no Projeto.</p>
					
					<p>De posse do seu cartão, o saque na conta-benefício poderá ser efetuado em qualquer Terminal de Autoatendimento (Caixa Eletrônico) do Banco do Brasil.</p>
					
					<p>Se porventura o seu cartão ainda não estiver disponível na agência nº {$arDados['agbanco']}, é possível efetuar o saque do benefício diretamente no 
					caixa de qualquer agência do Banco do Brasil. Você deve imprimir este comprovante e apresentá-lo ao caixa da agência.</p>
					
					<p>Nº do Convênio: 526</p>
					
					<p>Nº do Benefício: {$arDados['numero_ben']}</p>
					
					<p>CPF do Beneficiário: {$arDados['cpf']}</p>
					
					<p>&nbsp;</p>
					
					<b>OBS:</b> O crédito desta bolsa terá validade de 90 dias, expirando em {$arDados['dt_fim_validade']}, quando o valor será estornado pelo banco.
					
   					<p>&nbsp;</p>
   					
   					<p>Projeto Mais Médicos para o Brasil<br/> 
   					EBSERH - Empresa Brasileira de Serviços Hospitalares<br/> 
   					MEC - Ministério da Educação<br/></p>
   					";
   			
   			return $txt;
   		}
   		
   		public function enviaEmailValidarRemessaCredito($rmdid = null, $boEnvia = true)
   		{
   			
   			if($rmdid){
   				$sql = "select
   							tutnome,
   							tutemail,
   							tuttipo,
   							apgmes,
   							apgano,
   							tutagencia,
   							tutcpf,
   							nu_nib,
   							to_char(to_date(dt_fim_validade,'YYYYMMDD'),'DD/MM/YYYY') as dt_fim_validade
   						from maismedicos.remessadetalhe d
   						join maismedicos.tutor t on d.tutid = t.tutid
   						join maismedicos.autorizacaopagamento a ON a.rmdid = d.rmdid			   			
   						where d.rmdid = $rmdid";
   				
   				$arDetalhe = $this->pegaLinha($sql);
   				
   				$arDados['nome'] 		= $arDetalhe['tutnome'] ? $arDetalhe['tutnome'] : 'não informado';
   				$arDados['email'] 		= $arDetalhe['tutemail'] ? $arDetalhe['tutemail'] : 'não informado';
   				$arDados['tipobolsa'] 	= $arDetalhe['tuttipo'] ? ($arDetalhe['tuttipo'] == 'T' ? 'tutoria' : 'supervisão' ) : 'não informado';
   				$arDados['mesano'] 		= $arDetalhe['apgmes'] ? $arDetalhe['apgmes'].'/'.$arDetalhe['apgano'] : 'não informado';
   				$arDados['valor'] 		= $arDetalhe['tuttipo'] ? ($arDetalhe['tuttipo'] == 'T' ? '5.000,00' : '4.000,00') : 'não informado';
   				$arDados['vlrextenso']	= $arDetalhe['tuttipo'] ? valorMonetarioExtenso($arDetalhe['tuttipo'] == 'T' ? 5000.00 : 4000.00) : '(por extenso)';
   				$arDados['agbanco']		= $arDetalhe['tutagencia'] ? $arDetalhe['tutagencia'] : 'não informado';
   				$arDados['cpf']			= $arDetalhe['tutcpf'] ? formatar_cpf($arDetalhe['tutcpf']) : 'não informado';
   				$arDados['numero_ben']	= $arDetalhe['nu_nib'] ? $arDetalhe['nu_nib'] : 'não informado';
   				$arDados['dt_fim_validade']	= $arDetalhe['dt_fim_validade'] ? $arDetalhe['dt_fim_validade'] : 'não informado';
   				
   				$conteudo = $this->conteudoEmailValidarRemessaCredito($arDados);
   			}
   			
   			if($boEnvia){
   				
	   			require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	   			require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	   			
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $this;
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
	   					
				$mensagem->From     = 'noReply@mec.gov.br';
				$mensagem->FromName = SIGLA_SISTEMA;

				if(in_array($_SESSION['baselogin'], array('simec_desenvolvimento', 'simec_espelho_producao'))){
					$mensagem->AddAddress( 'henrique.couto@ebserh.gov.br', 'Henrique Xavier Couto'  );
					$mensagem->AddAddress( $_SESSION['usuemail'], $_SESSION['usunome']  );
					$mensagem->AddAddress( 'wescley.lima@ebserh.gov.br', 'Wescley Guedes Lima'  );
				}else{
					$mensagem->AddAddress( $arDados['email'], $arDados['nome']  );
				}
				
				$mensagem->Subject = SIGLA_SISTEMA. ' - Mais Médicos - Bolsa '.ucwords($arDados['tipobolsa']); //assunto
				$mensagem->Body = $conteudo; //conteudo
				$mensagem->IsHTML( true );
				
				if($mensagem->Send()){				
					$sql = "update maismedicos.remessadetalhe set bo_email_enviado = true, dt_envio_email = now() where rmdid = {$rmdid}";
					$this->executar($sql);
					$this->commit();
				}
   			}
   			
   			return $conteudo;
   		}
   
}