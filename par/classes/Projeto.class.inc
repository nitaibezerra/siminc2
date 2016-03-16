<?php

include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';

class Projeto{
	private $subacoes_generico_par, $obras_par, $obras_pac_quadra, $obras_pac_proinfancia, $obras_pac_proinfancia_Metod_inovadora, $obras_pac_quadra_Metod_inovadora, 
			$subacoes_par_emendas, $emendas_parlamentares, $emendas_obras_par, $brasil_pro_obras, $brasil_pro_generico, $esfera, $cnpjEntidade, $wsusuario, $wssenha, 
			$tipoProcesso, $numeroProcesso, $tipoExecucao, $arrPost, $tpProcesso;
	
	public function __construct( $arrPost = array() ){
		global $db;
		if( $arrPost['tipo'] && is_numeric($arrPost['tipo']) ){
			$arrTipo = $db->pegaLinha( "SELECT sisid, tprid, tipprogramafnde, tipnumerosistemasigef, tiptipoprocessosigef
											FROM execucaofinanceira.tipoprocesso WHERE tipid = {$arrPost['tipo']} and tipstatus = 'A'" ); 
		}
															
		
		$this->subacoes_generico_par					= 1;
		$this->obras_par 								= 2;
		$this->obras_pac_quadra							= 3;
		$this->subacoes_par_emendas 					= 4;
		$this->emendas_obras_par 						= 5;
		$this->obras_pac_proinfancia					= 6;
		$this->obras_pac_proinfancia_Metod_inovadora 	= 7;
		$this->obras_pac_quadra_Metod_inovadora			= 8;
		$this->subacoes_brasil_pro						= 9;
		$this->obras_par_brasil_pro						= 10;
		
		$this->wsusuario								= $arrPost['wsusuario'];
		$this->wssenha									= $arrPost['wssenha'];
		$this->tipoProcesso								= $arrPost['tipo'];
		$this->tpProcesso								= $arrTipo['tiptipoprocessosigef'];
		$this->tipoExecucao								= $arrTipo['tprid'];
		$this->sistema									= $arrTipo['sisid'];
		$this->programafnde								= $arrTipo['tipprogramafnde'];
		$this->tipnumerosistemasigef					= $arrTipo['tipnumerosistemasigef'];
		$this->arrPost									= $arrPost;

		
		$inuid = $_SESSION['par']['inuid'] ? $_SESSION['par']['inuid'] : $_SESSION['par_var']['inuid'];
		$_SESSION['par']['inuid'] = $inuid;
		
		if( !$inuid ){
			echo "<script>alert('Falta o INUID!');history.back(-1);</script>";
			exit;
		}

		/* Alterado por Eduardo DUnice - 26/05/2014
		 * Demanda PAr 481
		* CNPJ carregado vazio
		* */
		if( !empty($arrPost['iuecnpj']) ){
// 			$sql = "SELECT iue.iuecnpj, case when iu.itrid = 1 then 'E' else case when iu.inuid = 1 then 'E' else 'M' end end as esfera 
// 					FROM par.instrumentounidade iu
// 						inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
// 					WHERE
// 						iue.iuecnpj = '{$arrPost['iuecnpj']}'
// 					    and iue.iuestatus = 'A'";
			$sql = "SELECT
						coalesce(ent.entnumcpfcnpj, iue.iuecnpj) as iuecnpj, 
						case when iu.itrid = 1 then 'E' else case when iu.inuid = 1 then 'E' else 'M' end end as esfera
					FROM par.instrumentounidade iu
					INNER JOIN par.instrumentounidadeentidade 	iue ON iue.inuid = iu.inuid
					INNER JOIN par.entidade 					ent ON ent.inuid = iu.inuid AND entstatus = 'A' 
																		AND ( (dutid = 6 AND ( iu.itrid <> 1 AND iu.inuid <> 1 ) ) 
																				OR (dutid = 9 AND (iu.itrid = 1 AND iu.inuid = 1 ) ) )
					WHERE
						(iue.iuecnpj = '{$arrPost['iuecnpj']}' OR ent.entnumcpfcnpj = '{$arrPost['iuecnpj']}')
						AND iue.iuestatus = 'A'";
			$arrInstrumento		= $db->pegaLinha( $sql );
		} else {
			$sql = "SELECT
						coalesce(ent.entnumcpfcnpj, iue.iuecnpj) as iuecnpj, 
						case when iu.itrid = 1 then 'E' else case when iu.inuid = 1 then 'E' else 'M' end end as esfera
					FROM par.instrumentounidade iu
					INNER JOIN par.instrumentounidadeentidade 	iue ON iue.inuid = iu.inuid
					INNER JOIN par.entidade 					ent ON ent.inuid = iu.inuid AND entstatus = 'A' 
																		AND ( (dutid = 6 AND ( iu.itrid <> 1 AND iu.inuid <> 1 ) ) 
																				OR (dutid = 9 AND (iu.itrid = 1 OR iu.inuid = 1 ) ) )
					WHERE
						iu.inuid = {$_SESSION['par']['inuid']}
						AND iue.iuestatus = 'A'
						AND iue.iuedefault = true";
			
// 			$sql = "SELECT iue.iuecnpj, case when iu.itrid = 1 then 'E' else case when iu.inuid = 1 then 'E' else 'M' end end as esfera 
// 					FROM par.instrumentounidade iu
// 						inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
// 					WHERE
// 						iu.inuid = {$_SESSION['par']['inuid']}
// 					    and iue.iuestatus = 'A'
// 					    and iue.iuedefault = true";
					    
			$arrInstrumento		= $db->pegaLinha( $sql );
		}
// 		ver($sql);
		/* FIM - Alterado por Eduardo DUnice - 26/05/2014
		 * Demanda PAr 481
		 * CNPJ carregado vazio
		 * */
		$this->cnpjEntidade	= $arrInstrumento['iuecnpj'];
		$this->esfera		= $arrInstrumento['esfera'];
	}
	
	// Função que inativa o processo do PAR
	function inativaProcessoPar( $prp, $nprocesso = '' )
	{
		global $db;
		$sql = "UPDATE par.processopar SET prpstatus = 'I' WHERE prpid = {$prp}";
		
		if($db->executar( $sql ))
		{
			$sql2 = "UPDATE par.processoparcomposicao SET ppcstatus = 'I' WHERE prpid = {$prp}";
			if($db->executar( $sql2))
			{
				$db->commit();
				echo "Processo  PAR nº{$nprocesso} inativado com sucesso";
			}
			else
			{
			echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
			}
				
		}
		else
		{
		echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
		}
	}
	
	
	// Função que inativa o processo de obras do PAR
	function inativaProcessoParObras($pro, $nprocesso = '')
	{
		global $db;
		$sql = "UPDATE  par.processoobraspar SET prostatus = 'I' WHERE proid = {$pro}";
		
		if($db->executar( $sql ))
		{
			$sql2 = "UPDATE  par.processoobrasparcomposicao SET pocstatus = 'I' WHERE proid = {$pro}";
			if($db->executar( $sql2))
			{
				$db->commit();
				echo "Processo Obras PAR nº {$nprocesso} inativado com sucesso";
			}
			else
			{
				echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
			}
		}
		else
		{
			echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
		}
	}

	// Função que inativa o processo obras do PAC
	function inativaProcessoPac($pro, $nprocesso = '')
	{
		global $db;
		$sql = "UPDATE par.processoobra SET prostatus = 'I' WHERE proid = {$pro}";
		
		if($db->executar( $sql ))
		{
			$sql2 = "UPDATE  par.processoobraspaccomposicao SET pocstatus = 'I' WHERE proid =  {$pro}";
			if($db->executar( $sql2))
			{
				$db->commit();
				echo "Processo Obras PAC nº {$nprocesso} inativado com sucesso";
			}
			else
			{
				echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
			}
		}
		else
		{
			echo "Houve um erro ao tentar cancelar o processo Obras PAR nº {$nprocesso}";
		}
	}
	
	public function carregaDadosAquisicao(){
		global $db;
		
		if( $this->tipoProcesso == $this->subacoes_generico_par ){ #PAR AQUISICAO
			
			$titulo = "Lista de Subações PAR Aquisição";
			$filtro = '13, 6'; 
			$cabecalho = array("&nbsp;Ações&nbsp;", "Subação", "PTRES", "PI", "Valor da Subação", "Ano" );
			$sql = $this->listaSubacaoGenericaPAR( $filtro );
		}
		elseif( $this->tipoProcesso == $this->obras_par ) { #OBRAS PAR
			
			$titulo = "Lista de Subações Obras PAR";
			$filtro = '11';
			$cabecalho = array("&nbsp;Ações&nbsp;","Nome da obra","Tipo da Obra","Município","UF","Situação","Valor da Obra","Ano" );
			$sql = $this->listaObrasPAR( $filtro ); 
		}
		elseif( $this->tipoProcesso == $this->emendas_obras_par ){ #EMENDAS_OBRAS_PAR
			
			$titulo = "Lista de Obras de Emendas no PAR";
			$filtro = '15'; 
			$cabecalho = array("&nbsp;Ações&nbsp;","Nome da obra","Tipo da Obra","Município","UF","Situação","Valor da Obra","Ano" );
			$sql = $this->listaObrasEmendasPAR( $filtro );
		}
		elseif( $this->tipoProcesso == $this->subacoes_par_emendas ){ #EMENDAS
			
			$titulo = "Lista de Subações de Emendas no PAR";
			$filtro = '14, 15'; 
			$cabecalho = array("&nbsp;Ações&nbsp;", "Subação", "PTRES", "PI", "Valor da Subação", "Ano" );
			$sql = $this->listaSubacaoGenericaPAR( $filtro );
		}
		elseif( $this->tipoProcesso == $this->obras_pac_quadra){ #OBRAS PAC
			
			$titulo = "Lista de Obras do PAC - Quadras";
			$filtro = '1';
			$classsificacao = "Q', 'C";
			$cabecalho = array("&nbsp;Ações&nbsp;", "Nome da Obra", "Tipo da Obra", "Município", "UF", "Situação", "Valor da Obra", "Ano" );
			$sql = $this->listaObrasPAC( $filtro, $classsificacao );
		}
		elseif( $this->tipoProcesso == $this->obras_pac_proinfancia){
			
			$titulo = "Lista de Obras do PAC - Proinfância";
			$filtro = '1';
			$classsificacao = 'P';
			$cabecalho = array("&nbsp;Ações&nbsp;", "Nome da Obra", "Tipo da Obra", "Município", "UF", "Situação", "Valor da Obra", "Ano" );
			$sql = $this->listaObrasPAC( $filtro, $classsificacao );
		}
		elseif( $this->tipoProcesso == $this->obras_pac_proinfancia_Metod_inovadora){ #OBRAS PAC
			
			$titulo = "Lista de Obras do PAC - Proinfância";
			$filtro = '1';
			$ptoid = '44,45,43,42';
			$classsificacao = 'P';
			$cabecalho = array("&nbsp;Ações&nbsp;", "Nome da Obra", "Tipo da Obra", "Município", "UF", "Situação", "Valor da Obra", "Ano" );
			$sql = $this->listaObrasPAC( $filtro, $classsificacao, $ptoid );
		}
		elseif( $this->tipoProcesso == $this->obras_pac_quadra_Metod_inovadora){ #OBRAS PAC
			
			$titulo = "Lista de Obras do PAC - Quadras";
			$filtro = '1';
			$ptoid = '44,45,43,42';
			$classsificacao = 'Q';
			$cabecalho = array("&nbsp;Ações&nbsp;", "Nome da Obra", "Tipo da Obra", "Município", "UF", "Situação", "Valor da Obra", "Ano" );
			$sql = $this->listaObrasPAC( $filtro, $classsificacao, $ptoid );
		}
		elseif( $this->tipoProcesso == $this->obras_par_brasil_pro ) { #OBRAS PAR
			
			$titulo = "Lista de Obras do PAR - Brasil-Pró";
			$filtro = '16, 17'; 
			$cabecalho = array("&nbsp;Ações&nbsp;","Nome da obra","Tipo da Obra","Município","UF","Situação","Valor da Obra","Ano" );
			$sql = $this->listaObrasBrasilPro( $filtro ); 
		}
		elseif( $this->tipoProcesso == $this->subacoes_brasil_pro){ #Subações do Brasil-Pró
			
			$titulo = "Lista de Subações do Brasil-Pró";
			$filtro = '16, 17'; 
			$cabecalho = array("&nbsp;Ações&nbsp;", "Subação", "PTRES", "PI", "Valor da Subação", "Ano" );
			$sql = $this->listaSubacaoBrasilPro( $filtro );
		} else {
			$sql = array();
		}
		print '<table cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';
		print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.$titulo.'</label></td></tr><tr>';
		print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
		//ver($sql,d);
		$db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S');
	}
	
	public function consultaDadosContaCorrenteFNDE($codigo){
		global $db;
		
		$usuario 				= $this->wsusuario; 
		$senha   				= $this->wssenha;
		$nu_identificador 		= $this->cnpjEntidade;
		$ptrnumprocessoempenho 	= $this->numeroProcesso;
		$somente_conta_ativa	= "S";
		$numero_de_linhas		= "200";
		$date = date("c");
				
		try {
    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$date}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_identificador</nu_identificador>
			<nu_processo>$ptrnumprocessoempenho</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
					->execute();
	
			$xmlRetorno = $xml;
	
			$xml = simplexml_load_string( stripslashes($xml));
			
			if ( (int) $xml->status->result ){
				$obContaCorrenteWS  = $xml->body->row->children();
				$status 			= (string)	$obContaCorrenteWS->status;
				$co_status			= substr( $status, 0, 1 );
				
				if( trim($co_status) != 0 ){
					$this->WSHistoricoProcesso( $codigo, $arqXml, $xmlRetorno, 'GERAPROC - AtualizarContaCorrenteFNDE - Sucesso' );
					return $obContaCorrenteWS;
				} else {
					$this->WSHistoricoProcesso( $codigo, $arqXml, $xmlRetorno, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro' );
					//echo $this->mensagem('Consultar Conta Corrente no SIGEF', $status);
					return false;				
					die;
				}
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
		    	$erroMSG = str_replace( "'", '"', $erroMSG );
		    	$this->WSHistoricoProcesso( $codigo, $arqXml, $xml, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro' );
				echo $this->mensagem('Consultar Conta Corrente no SIGEF', $erroMSG);			
				die;
			}
			
		} catch (Exception $e){
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo $this->mensagem('Consultar Conta Corrente no SIGEF', $erro);			
				die;
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				echo $this->mensagem('Consultar Conta Corrente no SIGEF', $erroMSG);			
				die;
			}
			$this->WSHistoricoProcesso( $codigo, $arqXml, $xml, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro' );
		}
	}
	
	public function solicitaAberturaContaCorrente($codigo, $dados){
		global $db;
		
		$usuario 			= $this->arrPost['wsusuario'];
		$senha   			= $this->arrPost['wssenha'];
		$tipo	 			= $this->arrPost['tipo'];
		$co_programa_fnde 	= $this->programafnde;
		$nu_sistema			= $this->tipnumerosistemasigef;
				
		$agencia = substr(trim($dados['agencia']),0,4);
		
		$nu_identificador 	= $this->cnpjEntidade;
		$tp_identificador 	= '1';
		$nu_processo 		= $this->numeroProcesso;
		$nu_banco 			= $dados['banco'];
		$nu_agencia 		= $agencia;
		$nu_conta_corrente 	= null;
		$tp_solicitacao 	= '01';
		$motivo_solicitacao	= '0032';
		$convenio_bb 		= null;
		$tp_conta 			= 'N';
		$date 				= date("c");
	
	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$date}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;
	
		try {
			if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';						
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
				
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
			
			if ( (int) $xml->status->result ){
				$arrRetorno = array('seq_solic_cr' => $xml->body->seq_solic_cr, 'nu_seq_conta' => $xml->body->nu_seq_conta);
				
				$this->WSHistoricoProcesso( $codigo, $arqXml, $xmlRetorno, 'GERAPROC - SolicitarContaCorrente - Sucesso' );
				return $arrRetorno;
		    } else {
		    	$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
		    	$erroMSG = str_replace( "'", '"', $erroMSG );
		    	
		    	$this->WSHistoricoProcesso( $codigo, $arqXml, $xmlRetorno, 'GERAPROC - SolicitarContaCorrente - Erro' );
		    	
		    	echo $this->mensagem('Solicitar Conta Corrente no SIGEF', $erroMSG);
				die;
		    }
		} catch (Exception $e){
						
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Solicitar Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				echo $this->mensagem('Solicitar Conta Corrente no SIGEF', $erro);			
				die;
			}
			
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			$this->WSHistoricoProcesso( $codigo, $arqXml, $xml, 'GERAPROC - SolicitarContaCorrente - Erro' );
			
			echo $this->mensagem('Solicitar Conta Corrente no SIGEF', $erroMSG);
			die;
		}
	}
	
	public function enviaEmailProjeto(){
		global $db;
	/*	
		$sql = "select distinct u.usuemail 
				from seguranca.perfil p
					inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod
				    inner join seguranca.usuario u on u.usucpf = pu.usucpf
				where 
					p.sisid = 23 
					and p.pflcod = ".PAR_PERFIL_EMPENHADOR."
				    and u.suscod = 'A'
				    and p.pflstatus = 'A'";
		
		$strEmailTo 	= $db->carregarColuna($sql);
*/
		//Regra alterada a pedido do Julio no dia 08/11/2012.
		$strEmailTo		= array(0 => 'lucia.pereira@fnde.gov.br',1 => 'ana@fnde.gov.br');
		$remetente 		= array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
		$strAssunto 	= 'EMISSÃO DE NOTA DE EMPENHO';
		$strMensagem	= 'O processo '.$this->numeroProcesso.' tem subações disponíveis.';
		$anexos 		= array();
		
		if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//if( $_SESSION['baselogin'] == "simec_desenvolvimento" ) return true;
			$strEmailTo = array('thiago.barbosa@mec.gov.br', 'wesley.silva@mec.gov.br');
			return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', $anexos);
		} else {
			return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', $anexos);
		}
	}
	
	public function insereProcessoPAR( $processo ){
		global $db;
		
		if( $processo ){			
			if( !empty($this->arrPost['chk'][0]) ){
				$proid = $db->pegaUm("select proid from execucaofinanceira.processos where pronumeroprocesso in (SELECT prpnumeroprocesso FROM par.processopar WHERE prpstatus = 'A' and prpid = $processo)");
				
				foreach ($this->arrPost['chk'] as $sbdid) {
					$arrSub = explode('_', $sbdid);					
					$sql = "INSERT INTO par.processoparcomposicao(prpid, sbdid) 
							VALUES ($processo, {$arrSub[0]})";
					$db->executar($sql);

					$sql = "INSERT INTO execucaofinanceira.processocomposicao(proid, sbdid, preid, ptrid, prcvalortotal) 
							VALUES ($proid, {$arrSub[0]}, null, null, {$arrSub[1]})";
					$db->executar( $sql );
				}
			}
			return true;
		} else {
			$arrSbdid = array();
			if( !empty($this->arrPost['chk'][0]) ){
				foreach ($this->arrPost['chk'] as $sbdid) {
					$arrD = explode('_', $sbdid);
					array_push($arrSbdid, $arrD[0] );	
				}
			}
			$boTem = $db->pegaUm("select count(prpid) from par.processoparcomposicao where ppcstatus = 'A' and sbdid in (".implode(',', $arrSbdid).")");
			
			if( $boTem == 0 ){
				
				$arrParamProcesso = array(
								'nu_cpf' 	=> $_SESSION['usucpf'],
								'ds_resumo' => 'Manutenção e desenvolvimento da educação',
								'co_assunto'=> '100',
								'nu_cnpj' 	=> $this->cnpjEntidade
							);
				$obProcesso = new ProcessoFNDE( $this->wsusuario, $this->wssenha );
				$arrProcesso = $obProcesso->gerarProcessoFNDE( $arrParamProcesso );
			
				if( $arrProcesso ){
					$this->numeroProcesso 	= $arrProcesso['processo'];
					$nu_banco 				= $arrProcesso['banco'];
					$nu_agencia				= $arrProcesso['agencia'];
					
					$arrParam = array('banco' => $nu_banco, 'agencia' => $nu_agencia);
					
					$muncod	= ($_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : 'null');
					
					$prptipo = ( ($this->tipoProcesso == $this->subacoes_brasil_pro) ? 'B' : 'O' );
					
					$sql = "INSERT INTO par.processopar(prpnumeroprocesso, muncod, prpbanco, prpagencia, usucpf, prptipo, inuid, prptipoexecucao, prpdocumenta, prpcnpj, sisid, prpdatainclusao, prpgeraproc)
							VALUES ('{$this->numeroProcesso}', {$muncod}, '{$nu_banco}', '{$nu_agencia}', 
									'{$_SESSION['usucpf']}', '$prptipo', {$_SESSION['par']['inuid']}, 'T', null, '{$this->cnpjEntidade}', {$this->sistema}, now(), true) returning prpid";
					$prpid = $db->pegaUm( $sql );
					
					$this->WSHistoricoProcesso( $prpid, $arrProcesso['arqxml'], $arrProcesso['xmlretorno'], 'GERAPROC - GerarProcesso - Sucesso' );
							
					if( $prpid ){
						$this->enviaEmailProjeto();
						if( !empty($this->arrPost['chk'][0]) ){
							foreach ($this->arrPost['chk'] as $sbdid) {
								$arrSub = explode('_', $sbdid);
								
								$sql = "INSERT INTO par.processoparcomposicao(prpid, sbdid) 
										VALUES ($prpid, {$arrSub[0]})";
								$db->executar($sql);	
							}
						}
						
						$arrParamProcesso = array(
										'an_processo' 			=> date("Y"),
								    	'nu_processo' 			=> $this->numeroProcesso,
								    	'tp_processo' 			=> $this->tpProcesso,
								    	'co_programa_fnde' 		=> $this->programafnde,
										'nu_sistema'			=> $this->tipnumerosistemasigef,
										'nu_cnpj_favorecido' 	=> $this->cnpjEntidade,
									);
						
						$res_sp = $obProcesso->solicitarProcesso($arrParamProcesso);
						if($res_sp){
							$arrConta = $this->consultaDadosContaCorrenteFNDE( $prpid );
							if( !$arrConta ){
								$retornoCC = $this->solicitaAberturaContaCorrente($prpid, $arrParam);
							} else {
								$seq_solic_cr	= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
								$seq_conta 		= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
								$retornoCC 		= array('seq_solic_cr' => $seq_solic_cr, 'nu_seq_conta' => $seq_conta);
							}
						} else {
							return false;
						}
						if( $retornoCC ){
							$res_acc = $this->atualizaContaCorrentePAR($prpid, $retornoCC);
							if( $res_acc ){						
								$this->salvarExecucaoFinanceiraPar($prpid);
								return true;
							} else {
								return false;
							}
						} else {
							return false;
						}
					} else {
						echo $this->mensagem('Criando Projeto', 'Não foi possível inserir o processo.');
						return false;
						die;
					}
				}
			} else {
				echo $this->mensagem('Criando Projeto', 'Não foi possível inserir o processo. Processo inserido anteriormente.');
				return false;
				die;
			}
		}
	}
	
	public function insereProcessoObrasPAR( $processo ){
		global $db;
		
		if( $processo ){			
			if( !empty($this->arrPost['chk'][0]) ){
				$proid = $db->pegaUm("select proid from execucaofinanceira.processos p where p.pronumeroprocesso in (SELECT pronumeroprocesso FROM par.processoobraspar WHERE prostatus = 'A' and proid = $processo)");
				
				foreach ($this->arrPost['chk'] as $sbdid) {
					$arrSub = explode('_', $sbdid);					
					$sql = "INSERT INTO par.processoobrasparcomposicao(proid, preid) 
									VALUES ($processo, {$arrSub[0]})";
					$db->executar($sql);

					$sql = "INSERT INTO execucaofinanceira.processocomposicao(proid, sbdid, preid, ptrid, prcvalortotal) 
							VALUES ($proid, null, {$arrSub[0]}, null, {$arrSub[1]})";
					$db->executar( $sql );
				}
			}
			return true;
		} else {
			$arrpreid = array();
			if( !empty($this->arrPost['chk'][0]) ){
				foreach ($this->arrPost['chk'] as $preid) {
					$arrD = explode('_', $preid);
					array_push($arrpreid, $arrD[0] );	
				}
			}
			$boTem = $db->pegaUm("select count(proid) from par.processoobrasparcomposicao where pocstatus = 'A' and preid in (".implode(',', $arrpreid).")");
			
			if( $boTem == 0 ){
				$arrParamProcesso = array(
						'nu_cpf' 	=> $_SESSION['usucpf'],
						'ds_resumo' => 'Manutenção e desenvolvimento da educação',
						'co_assunto'=> '100',
						'nu_cnpj' 	=> $this->cnpjEntidade
				);
				$obProcesso = new ProcessoFNDE( $this->wsusuario, $this->wssenha );
				$arrProcesso = $obProcesso->gerarProcessoFNDE( $arrParamProcesso );
			
				if( $arrProcesso ){
					$this->numeroProcesso 	= $arrProcesso['processo'];
					$nu_banco 				= $arrProcesso['banco'];
					$nu_agencia				= $arrProcesso['agencia'];
					
					$arrParam = array('banco' => $nu_banco, 'agencia' => $nu_agencia);
					$estuf	= ($_SESSION['par']['estuf'] ? "'".$_SESSION['par']['estuf']."'" : 'null');
					$muncod	= ($_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : 'null');
					
					$protipo = ( ($this->tipoProcesso == $this->obras_par_brasil_pro) ? 'B' : 'P' );
					
					$sql = "INSERT INTO par.processoobraspar(pronumeroprocesso, inuid, muncod, resid, probanco, proagencia, prodatainclusao, usucpf, protipo, estuf, procnpj, progeraproc, sisid) 
							VALUES ('{$this->numeroProcesso}', {$_SESSION['par']['inuid']}, {$muncod}, null, '{$nu_banco}', '{$nu_agencia}', now(), 
									'{$_SESSION['usucpf']}', '{$protipo}', {$estuf}, '{$this->cnpjEntidade}', true, {$this->sistema}) returning proid";
					
					$proid = $db->pegaUm( $sql );
					$arrParam['proid']	= $proid;
					
					$this->WSHistoricoProcesso( $proid, $arrProcesso['arqxml'], $arrProcesso['xmlretorno'], 'GERAPROC - GerarProcesso - Sucesso' );
					
					if( $proid ){
						$this->enviaEmailProjeto();
						if( !empty($this->arrPost['chk'][0]) ){
							foreach ($this->arrPost['chk'] as $preid) {
								$arrSub = explode('_', $preid);						
								$sql = "INSERT INTO par.processoobrasparcomposicao(proid, preid) 
										VALUES ($proid, {$arrSub[0]})";
								$db->executar($sql);
							}
						}
						$arrParamProcesso = array(
								'an_processo' 			=> date("Y"),
								'nu_processo' 			=> $this->numeroProcesso,
								'tp_processo' 			=> $this->tpProcesso,
								'co_programa_fnde' 		=> $this->programafnde,
								'nu_sistema'			=> $this->tipnumerosistemasigef,
								'nu_cnpj_favorecido' 	=> $this->cnpjEntidade,
						);
						
						$res_sp = $obProcesso->solicitarProcesso($arrParamProcesso);
						if($res_sp){
							$arrConta = $this->consultaDadosContaCorrenteFNDE( $proid );
							if( !$arrConta ){
								$retornoCC = $this->solicitaAberturaContaCorrente($proid, $arrParam);
							} else {
								$seq_solic_cr	= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
								$seq_conta 		= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
								$retornoCC 		= array('seq_solic_cr' => $seq_solic_cr, 'nu_seq_conta' => $seq_conta);
							}
						} else {
							return false;
						}
						if( $retornoCC ){
							$res_acc = $this->atualizaContaCorrenteObrasPAR($proid, $retornoCC);
							if( $res_acc ){						
								$this->salvarExecucaoFinanceiraObrasPAR($proid);
								return true;
							} else {
								return false;
							}
						} else {
							return false;
						}
					} else {
						echo $this->mensagem('Criando Projeto', 'Não foi possível inserir o processo.');
						return false;
						die;
					}
				}
			} else {
				echo $this->mensagem('Criando Projeto', 'Não foi possível inserir o processo. Processo inserido anteriormente.');
				return false;
				die;
			}
		}
	}
	
	public function insereProcessoObrasPAC( $processo ){
		global $db;
		
		if( $processo ){			
			if( !empty($this->arrPost['chk'][0]) ){
				$proid = $db->pegaUm("select proid from execucaofinanceira.processos p where p.pronumeroprocesso in (SELECT pronumeroprocesso FROM par.processoobra WHERE prostatus = 'A' and proid = $processo)");
				
				foreach ($this->arrPost['chk'] as $sbdid) {
					$arrSub = explode('_', $sbdid);					
					$sql = "INSERT INTO par.processoobraspaccomposicao(proid, preid) 
									VALUES ($processo, {$arrSub[0]})";
					$db->executar($sql);

					$sql = "INSERT INTO execucaofinanceira.processocomposicao(proid, sbdid, preid, ptrid, prcvalortotal) 
							VALUES ($proid, null, {$arrSub[0]}, null, {$arrSub[1]})";
				
					$db->executar( $sql );
				}
			}
			return true;
		} else {
			
			$arrpreid = array();
			if( !empty($this->arrPost['chk'][0]) ){
				foreach ($this->arrPost['chk'] as $preid) {
					$arrD = explode('_', $preid);
					array_push($arrpreid, $arrD[0] );	
				}
			}
			$boTem = $db->pegaUm("select count(proid) from par.processoobraspaccomposicao where pocstatus = 'A' and  preid in (".implode(',', $arrpreid).")");
			
			if( $boTem == 0 ){
			
				$arrParamProcesso = array(
								'nu_cpf' 	=> $_SESSION['usucpf'],
								'ds_resumo' => 'Manutenção e desenvolvimento da educação',
								'co_assunto'=> '100',
								'nu_cnpj' 	=> $this->cnpjEntidade
							);
				$obProcesso = new ProcessoFNDE( $this->wsusuario, $this->wssenha );
				$arrProcesso = $obProcesso->gerarProcessoFNDE( $arrParamProcesso );
				
				if( $arrProcesso ){
					$this->numeroProcesso 	= $arrProcesso['processo'];
					$nu_banco 				= $arrProcesso['banco'];
					$nu_agencia				= $arrProcesso['agencia'];
					
					$arrParam = array('banco' => $nu_banco, 'agencia' => $nu_agencia);
					$estuf	= ($_SESSION['par']['estuf'] ? "'".$_SESSION['par']['estuf']."'" : 'null');
					$muncod	= ($_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : 'null');
					$tipoobras = explode('_', $this->arrPost['chk'][0]);
					$protipo = $tipoobras[2];
					
					$sql = "INSERT INTO par.processoobra(pronumeroprocesso, muncod, resid, probanco, proagencia, prodatainclusao, usucpf, protipo, estuf, procnpj, progeraproc) 
							VALUES ('{$this->numeroProcesso}', {$muncod}, null, '{$nu_banco}', '{$nu_agencia}', now(), 
									'{$_SESSION['usucpf']}', '{$protipo}', {$estuf}, '{$this->cnpjEntidade}', true) returning proid";
					
					$proid = $db->pegaUm( $sql );
					$arrParam['proid']	= $proid;
					
					$this->WSHistoricoProcesso( $proid, $arrProcesso['arqxml'], $arrProcesso['xmlretorno'], 'GERAPROC - GerarProcesso - Sucesso' );
					
					if( $proid ){
						$this->enviaEmailProjeto();
						if( !empty($this->arrPost['chk'][0]) ){
							foreach ($this->arrPost['chk'] as $preid) {
								$arrSub = explode('_', $preid);						
								$sql = "INSERT INTO par.processoobraspaccomposicao(proid, preid) 
										VALUES ($proid, {$arrSub[0]})";
								$db->executar($sql);
							}
						}
						$arrParamProcesso = array(
										'an_processo' 			=> date("Y"),
								    	'nu_processo' 			=> $this->numeroProcesso,
								    	'tp_processo' 			=> $this->tpProcesso,
								    	'co_programa_fnde' 		=> $this->programafnde,
										'nu_sistema'			=> $this->tipnumerosistemasigef,
										'nu_cnpj_favorecido' 	=> $this->cnpjEntidade,
									);
						
						$res_sp = $obProcesso->solicitarProcesso($arrParamProcesso);
						if($res_sp){
							$arrConta = $this->consultaDadosContaCorrenteFNDE( $proid );
							if( !$arrConta ){
								$retornoCC = $this->solicitaAberturaContaCorrente($proid, $arrParam);
							} else {
								$seq_solic_cr	= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
								$seq_conta 		= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
								$retornoCC 		= array('seq_solic_cr' => $seq_solic_cr, 'nu_seq_conta' => $seq_conta);
							}
						} else {
							return false;
						}
						if( $retornoCC ){
							$res_acc = $this->atualizaContaCorrenteObrasPAC($proid, $retornoCC);
							if( $res_acc ){						
								$this->salvarExecucaoFinanceiraObrasPAC($proid);
								return true;
							} else {
								return false;
							}
						} else {
							return false;
						}
					} else {
						echo $this->mensagem('Criando Projeto', 'Não foi possível inserir o processo.');
						return false;
						die;
					}
				}
			} else {
				echo $this->mensagem('Criando Projeto', 'Não foi possível inserir o processo. Processo inserido anteriormente.');
				return false;
				die;
			}
		}
	}
		
	public function atualizaContaCorrentePAR( $prpid, $retornoCC, $arrConta = array() ){
		global $db;
		
		if( !$arrConta ){
			$sql = "UPDATE par.processopar SET prpseqconta='".$retornoCC['seq_solic_cr']."', seq_conta_corrente='".$retornoCC['nu_seq_conta']."' WHERE prpid = {$prpid}";
			$db->executar($sql);			
			$arrConta = $this->consultaDadosContaCorrenteFNDE( $prpid );
		}		
		$seq_solic_cr 		= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
		$seq_conta 			= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
		$dt_movimento 		= !empty($arrConta->dt_movimento) ? (int)$arrConta->dt_movimento : 'null';
		$nu_banco 			= !empty($arrConta->nu_banco) ? "'".$arrConta->nu_banco."'" : 'null';
		$nu_agencia 		= !empty($arrConta->nu_agencia) ? "'".$arrConta->nu_agencia."'" : 'null';
		$nu_conta_corrente	= !empty($arrConta->nu_conta_corrente) ? "'".$arrConta->nu_conta_corrente."'" : 'null';
		$fase_solicitacao	= !empty($arrConta->fase_solicitacao) ? "'".$arrConta->fase_solicitacao."'" : 'null';
		$co_situacao_conta	= !empty($arrConta->co_situacao_conta) ? (int)$arrConta->co_situacao_conta : 'null';
		$situacao_conta 	= !empty($arrConta->situacao_conta) ? "'".$arrConta->situacao_conta."'" : 'null';
		$nu_processo 		= !empty($arrConta->nu_processo) ? "'".$arrConta->nu_processo."'" : 'null';
		$nu_identificador 	= !empty($arrConta->nu_identificador) ? "'".$arrConta->nu_identificador."'" : 'null';
		$ds_razao_social 	= !empty($arrConta->ds_razao_social) ? "'".$arrConta->ds_razao_social."'" : 'null';
		
		$sql = "UPDATE par.processopar SET
					--prpseqconta = $seq_conta,
					--seq_conta_corrente = $seq_solic_cr,
					nu_conta_corrente = $nu_conta_corrente,
					dt_movimento = $dt_movimento,
					fase_solicitacao = $fase_solicitacao,
					co_situacao_conta = $co_situacao_conta,
					situacao_conta = $situacao_conta,
					ds_razao_social = $ds_razao_social				 
				WHERE 
				  	prpid = $prpid";
		$db->executar( $sql );
		 return true;
	}
	
	public function atualizaContaCorrenteObrasPAR( $proid, $retornoCC, $arrConta = array() ){
		global $db;
		
		if( !$arrConta ){
			$sql = "UPDATE par.processoobraspar SET proseqconta = '".$retornoCC['seq_solic_cr']."', seq_conta_corrente = '".$retornoCC['nu_seq_conta']."' WHERE proid = {$proid}";
			$db->executar($sql);			
			$arrConta = $this->consultaDadosContaCorrenteFNDE( $proid );
		}		
		$seq_solic_cr 		= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
		$seq_conta 			= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
		$dt_movimento 		= !empty($arrConta->dt_movimento) ? (int)$arrConta->dt_movimento : 'null';
		$nu_banco 			= !empty($arrConta->nu_banco) ? "'".$arrConta->nu_banco."'" : 'null';
		$nu_agencia 		= !empty($arrConta->nu_agencia) ? "'".$arrConta->nu_agencia."'" : 'null';
		$nu_conta_corrente	= !empty($arrConta->nu_conta_corrente) ? "'".$arrConta->nu_conta_corrente."'" : 'null';
		$fase_solicitacao	= !empty($arrConta->fase_solicitacao) ? "'".$arrConta->fase_solicitacao."'" : 'null';
		$co_situacao_conta	= !empty($arrConta->co_situacao_conta) ? (int)$arrConta->co_situacao_conta : 'null';
		$situacao_conta 	= !empty($arrConta->situacao_conta) ? "'".$arrConta->situacao_conta."'" : 'null';
		$nu_processo 		= !empty($arrConta->nu_processo) ? "'".$arrConta->nu_processo."'" : 'null';
		$nu_identificador 	= !empty($arrConta->nu_identificador) ? "'".$arrConta->nu_identificador."'" : 'null';
		$ds_razao_social 	= !empty($arrConta->ds_razao_social) ? "'".$arrConta->ds_razao_social."'" : 'null';
		
		$sql = "UPDATE par.processoobraspar SET nu_conta_corrente = $nu_conta_corrente WHERE proid = $proid";
		$db->executar( $sql );
		return true;
	}
	
	public function atualizaContaCorrenteObrasPAC( $proid, $retornoCC, $arrConta = array() ){
		global $db;
		
		if( !$arrConta ){
			$sql = "UPDATE par.processoobra SET proseqconta = '".$retornoCC['seq_solic_cr']."', seq_conta_corrente = '".$retornoCC['nu_seq_conta']."' WHERE proid = {$proid}";
			$db->executar($sql);			
			$arrConta = $this->consultaDadosContaCorrenteFNDE( $proid );
		}		
		$seq_solic_cr 		= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
		$seq_conta 			= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
		$dt_movimento 		= !empty($arrConta->dt_movimento) ? (int)$arrConta->dt_movimento : 'null';
		$nu_banco 			= !empty($arrConta->nu_banco) ? "'".$arrConta->nu_banco."'" : 'null';
		$nu_agencia 		= !empty($arrConta->nu_agencia) ? "'".$arrConta->nu_agencia."'" : 'null';
		$nu_conta_corrente	= !empty($arrConta->nu_conta_corrente) ? "'".$arrConta->nu_conta_corrente."'" : 'null';
		$fase_solicitacao	= !empty($arrConta->fase_solicitacao) ? "'".$arrConta->fase_solicitacao."'" : 'null';
		$co_situacao_conta	= !empty($arrConta->co_situacao_conta) ? (int)$arrConta->co_situacao_conta : 'null';
		$situacao_conta 	= !empty($arrConta->situacao_conta) ? "'".$arrConta->situacao_conta."'" : 'null';
		$nu_processo 		= !empty($arrConta->nu_processo) ? "'".$arrConta->nu_processo."'" : 'null';
		$nu_identificador 	= !empty($arrConta->nu_identificador) ? "'".$arrConta->nu_identificador."'" : 'null';
		$ds_razao_social 	= !empty($arrConta->ds_razao_social) ? "'".$arrConta->ds_razao_social."'" : 'null';
		
		$sql = "UPDATE par.processoobra SET nu_conta_corrente = $nu_conta_corrente WHERE proid = $proid";
		$db->executar( $sql );
		return true;
	}
	
	public function salvarExecucaoFinanceiraPar( $prpid ){
		global $db;
		
		$tipid 	= $this->tipoProcesso;
		$esfera = $this->esfera;
		$tprid	= $this->tipoExecucao; #Convenio
		$ano	= date('Y');
		$estuf	= ($_SESSION['par']['estuf'] ? "'".$_SESSION['par']['estuf']."'" : 'null');
		$muncod	= ($_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : 'null');
		
		$sql = "INSERT INTO execucaofinanceira.processos(pronumeroprocesso, tprid, procnpj, proesfera, pronumeroconveniosigef, prodatainclusao,
	  				prodocumenta, pronumeroconveniosiafi, proano, prostatus, muncod, estuf, usucpf, tipid)   				
				(SELECT prpnumeroprocesso, $tprid, prpcnpj, '$esfera', prpnumeroconveniofnde, now(), 
					prpdocumenta, prpnumeroconveniosiafi, '$ano', 'A', {$muncod}, {$estuf}, usucpf, $tipid
				FROM par.processopar WHERE prpid = $prpid) returning proid";
		
		$proid = $db->pegaUm( $sql );
		
		if( !empty($this->arrPost['chk'][0]) ){
			foreach ($this->arrPost['chk'] as $sbdid) {
				$arrSub = explode('_', $sbdid);
				$sql = "INSERT INTO execucaofinanceira.processocomposicao(proid, sbdid, preid, ptrid, prcvalortotal) 
						VALUES ($proid, {$arrSub[0]}, null, null, {$arrSub[1]})";
				$db->executar( $sql );
			}				
		}
		
		$sql = "INSERT INTO execucaofinanceira.contacorrente(ccobanco, ccoagencia, ccoconta, ccosequencialcontasigef, ccostatus, ccodata, ccoseqsoliccrsigef,
	  				ccoseqcontasigef, ccodatamovimentosigef, ccofasesolicitacaosigef, ccocodigosituacaocontasigef, ccodescricaosituacaocontasigef, proid) 
				(SELECT prpbanco, prpagencia, nu_conta_corrente, prpseqconta, 'A', now(), prpseqconta, seq_conta_corrente, dt_movimento, fase_solicitacao, co_situacao_conta, situacao_conta, $proid
				FROM par.processopar WHERE prpid = $prpid)";
		$db->executar( $sql );
		return true;
	}
	
	public function salvarExecucaoFinanceiraObrasPAR( $codigo ){
		global $db;
		
		$tipid 	= $this->tipoProcesso;
		$esfera = $this->esfera;
		$tprid	= $this->tipoExecucao; #Convenio
		$ano	= date('Y');
		$estuf	= ($_SESSION['par']['estuf'] ? "'".$_SESSION['par']['estuf']."'" : 'null');
		$muncod	= ($_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : 'null');
		
		$sql = "INSERT INTO execucaofinanceira.processos(pronumeroprocesso, tprid, procnpj, proesfera, pronumeroconveniosigef, prodatainclusao,
	  				prodocumenta, pronumeroconveniosiafi, proano, prostatus, muncod, estuf, usucpf, tipid)   				
				(SELECT pronumeroprocesso, $tprid, procnpj, '$esfera', null, now(), null, null, ".date('Y').", 'A', $muncod, $estuf, usucpf, $tipid 
				FROM par.processoobraspar where prostatus = 'A' and proid = $codigo) returning proid";
		$proid = $db->pegaUm( $sql );
		
		if( !empty($this->arrPost['chk'][0]) ){
			foreach ($this->arrPost['chk'] as $sbdid) {
				$arrSub = explode('_', $sbdid);
				$sql = "INSERT INTO execucaofinanceira.processocomposicao(proid, sbdid, preid, ptrid, prcvalortotal) 
						VALUES ($proid, null, {$arrSub[0]}, null, {$arrSub[1]})";
				$db->executar( $sql );
			}				
		}
		$arrConta 			= $this->consultaDadosContaCorrenteFNDE( $codigo );
		$seq_solic_cr 		= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
		$seq_conta 			= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
		$dt_movimento 		= !empty($arrConta->dt_movimento) ? (int)$arrConta->dt_movimento : 'null';
		$nu_banco 			= !empty($arrConta->nu_banco) ? "'".$arrConta->nu_banco."'" : 'null';
		$nu_agencia 		= !empty($arrConta->nu_agencia) ? "'".$arrConta->nu_agencia."'" : 'null';
		$nu_conta_corrente	= !empty($arrConta->nu_conta_corrente) ? "'".$arrConta->nu_conta_corrente."'" : 'null';
		$fase_solicitacao	= !empty($arrConta->fase_solicitacao) ? "'".$arrConta->fase_solicitacao."'" : 'null';
		$co_situacao_conta	= !empty($arrConta->co_situacao_conta) ? (int)$arrConta->co_situacao_conta : 'null';
		$situacao_conta 	= !empty($arrConta->situacao_conta) ? "'".$arrConta->situacao_conta."'" : 'null';
		$nu_processo 		= !empty($arrConta->nu_processo) ? "'".$arrConta->nu_processo."'" : 'null';
		$nu_identificador 	= !empty($arrConta->nu_identificador) ? "'".$arrConta->nu_identificador."'" : 'null';
		$ds_razao_social 	= !empty($arrConta->ds_razao_social) ? "'".$arrConta->ds_razao_social."'" : 'null';
		
		$sql = "INSERT INTO execucaofinanceira.contacorrente(ccobanco, ccoagencia, ccoconta, ccosequencialcontasigef, ccostatus, ccodata, ccoseqsoliccrsigef,
	  				ccoseqcontasigef, ccodatamovimentosigef, ccofasesolicitacaosigef, ccocodigosituacaocontasigef, ccodescricaosituacaocontasigef, proid) 
				(SELECT probanco, proagencia, $nu_conta_corrente, proseqconta, 'A', now(), $seq_solic_cr, $seq_conta, $dt_movimento,
					$fase_solicitacao, $co_situacao_conta, $situacao_conta, $proid FROM par.processoobra WHERE prostatus = 'A' and proid = $codigo)";
		
		$db->executar( $sql );
		return true;
	}
	
	public function salvarExecucaoFinanceiraObrasPAC( $codigo ){
		global $db;
		
		$tipid 	= $this->tipoProcesso;
		$esfera = $this->esfera;
		$tprid	= $this->tipoExecucao; #Convenio
		$ano	= date('Y');
		$estuf	= ($_SESSION['par']['estuf'] ? "'".$_SESSION['par']['estuf']."'" : 'null');
		$muncod	= ($_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : 'null');
		
		$sql = "INSERT INTO execucaofinanceira.processos(pronumeroprocesso, tprid, procnpj, proesfera, pronumeroconveniosigef, prodatainclusao,
	  				prodocumenta, pronumeroconveniosiafi, proano, prostatus, muncod, estuf, usucpf, tipid)   				
				(SELECT pronumeroprocesso, $tprid, procnpj, '$esfera', null, now(), null, null, ".date('Y').", 'A', $muncod, $estuf, usucpf, $tipid 
				FROM par.processoobra where prostatus = 'A' and proid = $codigo) returning proid";
		$proid = $db->pegaUm( $sql );
		
		if( !empty($this->arrPost['chk'][0]) ){
			foreach ($this->arrPost['chk'] as $sbdid) {
				$arrSub = explode('_', $sbdid);
				$sql = "INSERT INTO execucaofinanceira.processocomposicao(proid, sbdid, preid, ptrid, prcvalortotal) 
						VALUES ($proid, null, {$arrSub[0]}, null, {$arrSub[1]})";
				$db->executar( $sql );
			}				
		}
		
		$arrConta 			= $this->consultaDadosContaCorrenteFNDE( $codigo );
		$seq_solic_cr 		= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
		$seq_conta 			= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
		$dt_movimento 		= !empty($arrConta->dt_movimento) ? (int)$arrConta->dt_movimento : 'null';
		$nu_banco 			= !empty($arrConta->nu_banco) ? "'".$arrConta->nu_banco."'" : 'null';
		$nu_agencia 		= !empty($arrConta->nu_agencia) ? "'".$arrConta->nu_agencia."'" : 'null';
		$nu_conta_corrente	= !empty($arrConta->nu_conta_corrente) ? "'".$arrConta->nu_conta_corrente."'" : 'null';
		$fase_solicitacao	= !empty($arrConta->fase_solicitacao) ? "'".$arrConta->fase_solicitacao."'" : 'null';
		$co_situacao_conta	= !empty($arrConta->co_situacao_conta) ? (int)$arrConta->co_situacao_conta : 'null';
		$situacao_conta 	= !empty($arrConta->situacao_conta) ? "'".$arrConta->situacao_conta."'" : 'null';
		$nu_processo 		= !empty($arrConta->nu_processo) ? "'".$arrConta->nu_processo."'" : 'null';
		$nu_identificador 	= !empty($arrConta->nu_identificador) ? "'".$arrConta->nu_identificador."'" : 'null';
		$ds_razao_social 	= !empty($arrConta->ds_razao_social) ? "'".$arrConta->ds_razao_social."'" : 'null';
		
		$sql = "INSERT INTO execucaofinanceira.contacorrente(ccobanco, ccoagencia, ccoconta, ccosequencialcontasigef, ccostatus, ccodata, ccoseqsoliccrsigef,
	  				ccoseqcontasigef, ccodatamovimentosigef, ccofasesolicitacaosigef, ccocodigosituacaocontasigef, ccodescricaosituacaocontasigef, proid) 
				(SELECT probanco, proagencia, $nu_conta_corrente, proseqconta, 'A', now(), $seq_solic_cr, $seq_conta, $dt_movimento,
					$fase_solicitacao, $co_situacao_conta, $situacao_conta, $proid FROM par.processoobra WHERE prostatus = 'A' and proid = $codigo)";
		
		$db->executar( $sql );
		return true;
	}
	
	public function WSHistoricoProcesso( $codigo = 'null', $arqXml, $xml, $msg = '' ){
		
		switch ($this->tipoProcesso) {
			case $this->subacoes_generico_par:
			case $this->subacoes_brasil_pro:
				$this->WSHistoricoProcessoPar( $codigo, $arqXml, $xml, $msg );
			break;
			case $this->subacoes_par_emendas:
				$this->WSHistoricoProcessoPar( $codigo, $arqXml, $xml, $msg );
			break;
			case $this->obras_par_brasil_pro:
			case $this->obras_par:
				$this->WSHistoricoProcessoObraPar( $codigo, $arqXml, $xml, $msg );
			break;
			case $this->emendas_obras_par:
				$this->WSHistoricoProcessoObraPar( $codigo, $arqXml, $xml, $msg );
			break;
			case $this->obras_pac_quadra:
 				$this->WSHistoricoProcessoObra( $codigo, $arqXml, $xml, $msg );
			break;
		}
	}
	
	public function WSHistoricoProcessoPar($prpid, $arqXml, $xmlRetorno, $msg = ''){
		global $db;
		
		$sql = "INSERT INTO par.historicowsprocessopar(
			    	prpid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$prpid."',
			    		'".$msg."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();
	}
	
	public function WSHistoricoProcessoObraPar($proid, $arqXml, $xmlRetorno, $msg = ''){
		global $db;
		
		$sql = "INSERT INTO par.historicowsprocessoobrapar(
			    	proid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$proid."',
			    		'".$msg."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();
	}
	
	public function WSHistoricoProcessoObra($proid, $arqXml, $xmlRetorno, $msg = ''){
		global $db;
		
		$sql = "INSERT INTO par.historicowsprocessoobra(
			    	proid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$proid."',
			    		'".$msg."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";
	
		$db->executar($sql);
		$db->commit();
	}
	
	public function mensagem( $title, $msg ){
		$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					'.$title.'
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
				</div>
			 	<br>';
		return $html;
	}
	
	public function solicitaContaCorrentePorProcesso($codigo, $arrParam){
		global $db;
		
		$this->numeroProcesso = $arrParam['processo'];
		$arrParam = array(
				'an_processo' 			=> date("Y"),
				'nu_processo' 			=> $this->numeroProcesso,
				'tp_processo' 			=> $this->tpProcesso,
				'co_programa_fnde' 		=> $this->programafnde,
				'nu_sistema'			=> $this->tipnumerosistemasigef,
				'nu_cnpj_favorecido' 	=> $this->cnpjEntidade,
		);
		
		$obProcesso = new ProcessoFNDE($this->wsusuario, $this->wssenha);
		$res_sp = $obProcesso->solicitarProcesso($arrParam);
		if($res_sp){
			$arrConta = $this->consultaDadosContaCorrenteFNDE( $codigo );
			if( !$arrConta ){
				$retornoCC = $this->solicitaAberturaContaCorrente($codigo, $arrParam);
			} else {
				$seq_solic_cr	= !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
				$seq_conta 		= !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
				$retornoCC 		= array('seq_solic_cr' => $seq_solic_cr, 'nu_seq_conta' => $seq_conta);
			}
			if( $retornoCC ){
				//$this->enviaEmailProjeto();
				
				switch ($this->tipoProcesso) {
					case $this->subacoes_generico_par:
					case $this->subacoes_brasil_pro:
						$res_acc = $this->atualizaContaCorrentePAR($codigo, $retornoCC, $arrConta);
						if( $res_acc ){
							$this->salvarExecucaoFinanceiraPar($codigo);
						}
					break;
					case $this->obras_par_brasil_pro:
					case $this->obras_par:
						$res_acc = $this->atualizaContaCorrenteObrasPAR($codigo, $retornoCC, $arrConta);
						if( $res_acc ){
							$this->salvarExecucaoFinanceiraObrasPAR($codigo);
						}
					break;
					case $this->emendas_obras_par:
						$res_acc = $this->atualizaContaCorrenteObrasPAR($codigo, $retornoCC, $arrConta);
						if( $res_acc ){
							$this->salvarExecucaoFinanceiraObrasPAR($codigo);
						}
					break;
					case $this->obras_pac_quadra:
						$res_acc = $this->atualizaContaCorrenteObrasPAC($codigo, $retornoCC, $arrConta);
						if( $res_acc ){
							$this->salvarExecucaoFinanceiraObrasPAC($codigo);
						}
					break;
					case $this->subacoes_par_emendas:
						$res_acc = $this->atualizaContaCorrentePAR($codigo, $retornoCC, $arrConta);
						if( $res_acc ){
							$this->salvarExecucaoFinanceiraPar($codigo);
						}
					break;
				}
				if( $res_acc ){
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	public function listaSubacaoGenericaPAR( $filtro, $itrid = '' ){
		//<a href=\"javascript:listarDados(\'' || sbdid || '\', \'' || sbdano || '\', \'' || inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=\"0\"></a>
		$sql = "SELECT DISTINCT
					'<center><input type=checkbox name=chk[] id=chk_'||sbdid||' value='||sbdid||'_'||valorsubacao||'></center>' as acoes,
						dados.descricaosubacao,
						dados.sbdptres||' ' as ptres,
						dados.sbdplanointerno,
						dados.valorsubacao,
						dados.sbdano||' ' as ano
				FROM (
						SELECT s.sbaid, s.sbadsc as descricaosubacao, sd.sbdano, iu.inuid, sd.sbdid, sd.sbdptres, sd.sbdplanointerno,
					  	coalesce((SELECT cast(par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano) as numeric(20,2))), 0.00) AS valorsubacao
					FROM par.instrumentounidade 	iu
					    INNER JOIN par.pontuacao 	po on po.inuid   = iu.inuid AND ptostatus <> 'I'
					    INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid 
					    INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'					
					    INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid
					WHERE
						iu.inuid = {$_SESSION['par']['inuid']} 
						AND sd.ssuid IN ( 2, 12  ) 
						AND s.frmid IN ( $filtro )
						/*".($itrid ? " and iu.itrid = $itrid " : '')."*/
						and sd.sbdptres is not null
                        and sd.sbdplanointerno is not null
						and sbdid not in (SELECT sbdid FROM par.processoparcomposicao where ppcstatus = 'A' )
		) AS dados";

		return $sql;
	}
	
	public function listaSubacaoBrasilPro( $filtro ){
		//<a href=\"javascript:listarDados(\'' || sbdid || '\', \'' || sbdano || '\', \'' || inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=\"0\"></a>
		$sql = "SELECT DISTINCT
					'<center><input type=checkbox name=chk[] id=chk_'||sbdid||' value='||sbdid||'_'||valorsubacao||'></center>' as acoes,
						dados.descricaosubacao,
						dados.sbdptres||' ' as ptres,
						dados.sbdplanointerno,
						dados.valorsubacao,
						dados.sbdano||' ' as ano
				FROM (
						SELECT s.sbaid, s.sbadsc as descricaosubacao, sd.sbdano, iu.inuid, sd.sbdid, sd.sbdptres, sd.sbdplanointerno,
							                coalesce((SELECT cast(par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano) as numeric(20,2))), 0.00) AS valorsubacao
							FROM par.instrumentounidade    iu
			                INNER JOIN par.pontuacao   po on po.inuid   = iu.inuid AND ptostatus <> 'I'
			                INNER JOIN par.criterio c on c.crtid = po.crtid
			                INNER JOIN par.indicador i on i.indid = c.indid
			                INNER JOIN par.area ar on ar.areid = i.areid
			                INNER JOIN par.dimensao d on d.dimid = ar.dimid and d.itrid = 3 
			                INNER JOIN par.acao               a  on a.ptoid   = po.ptoid 
			                INNER JOIN par.subacao            s  on s.aciid   = a.aciid AND sbastatus = 'A'                                  
			                INNER JOIN par.subacaodetalhe     sd ON sd.sbaid = s.sbaid
						WHERE
			                iu.inuid = {$_SESSION['par']['inuid']} 
			                and sd.ssuid IN (2, 12) 
			                and s.frmid IN ( $filtro )    
			                and sd.sbdptres is not null
			                and sd.sbdplanointerno is not null
			                and sbdid not in (SELECT sbdid FROM par.processoparcomposicao where ppcstatus = 'A' )
				) AS dados";

		return $sql;
	}
	
	public function listaObrasPAR( $filtro ){
		//<a href=\"javascript:listarDados(\'' || pe.preid || '\', \'' || pe.preano || '\', \'' || iu.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Obra\" border=\"0\"></a>
		
		$sql = "SELECT DISTINCT
				    '<center><input type=checkbox name=chk[] id=chk_'||pe.preid||' value='||pe.preid||'_'||COALESCE(pe.prevalorobra, 0)||'></center>' as acoes,
				    pe.predescricao as descricao,
				    pto.ptodescricao,
					mun.mundescricao,
					mun.estuf,
					esd.esddsc,
				  	COALESCE(pe.prevalorobra, 0) as valor,
				    pe.preano||' ' as ano 
				FROM par.instrumentounidade 	iu
				    INNER JOIN par.pontuacao 	po on po.inuid  = iu.inuid AND ptostatus <> 'I'
				    INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid
				    INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'					
				    inner join par.subacaoobra	sob on sob.sbaid 	= s.sbaid
				    inner join obras.preobra	pe on pe.preid	= sob.preid and pe.prestatus = 'A' 
				    INNER JOIN obras.pretipoobra pto ON pto.ptoid = pe.ptoid AND pto.ptostatus = 'A' 
				    INNER JOIN territorios.municipio mun ON mun.muncod = pe.muncod 
					INNER JOIN workflow.documento doc ON doc.docid = pe.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				WHERE
				    iu.inuid = {$_SESSION['par']['inuid']}  
				    AND s.frmid IN ( $filtro ) 
				    AND esd.esdid in (".WF_PAR_OBRA_APROVADA.", ".WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL.")
				    AND pto.tooid in (11, 2, 6)
				    AND pe.preid not in (SELECT preid FROM par.processoobrasparcomposicao where pocstatus = 'A' )
				    and pe.prevalorobra is not null
				    -- verificando se a obra não foi empenhado - solicitado pelo Daniel Areas -- 
				    and (select count(*) from par.empenhoobrapar where preid=pe.preid and eobstatus = 'A')=0
				order by 
					pe.predescricao";
		//ver($sql);
		return $sql;
	}
	
	public function listaObrasBrasilPro( $filtro ){
		//<a href=\"javascript:listarDados(\'' || pe.preid || '\', \'' || pe.preano || '\', \'' || iu.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Obra\" border=\"0\"></a>
		$sql = "SELECT DISTINCT
				    '<center><input type=checkbox name=chk[] id=chk_'||pe.preid||' value='||pe.preid||'_'||COALESCE(pe.prevalorobra, 0)||'></center>' as acoes,
				    pe.predescricao as descricao,
				    pto.ptodescricao,
					mun.mundescricao,
					mun.estuf,
					esd.esddsc,
				  	COALESCE(pe.prevalorobra, 0) as valor,
				    pe.preano||' ' as ano 
				FROM par.instrumentounidade 	iu
				    INNER JOIN par.pontuacao 	po on po.inuid  = iu.inuid AND ptostatus <> 'I'
				    INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid
				    INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'					
				    inner join par.subacaoobra	sob on sob.sbaid 	= s.sbaid
				    inner join obras.preobra	pe on pe.preid	= sob.preid and pe.prestatus = 'A' 
				    INNER JOIN obras.pretipoobra pto ON pto.ptoid = pe.ptoid AND pto.ptostatus = 'A' 
				    INNER JOIN territorios.municipio mun ON mun.muncod = pe.muncod 
					INNER JOIN workflow.documento doc ON doc.docid = pe.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				WHERE
				    iu.inuid = {$_SESSION['par']['inuid']}  
				    AND s.frmid IN ( $filtro ) 
				    AND esd.esdid in (".WF_PAR_OBRA_APROVADA.", ".WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL.")
				    AND pto.tooid in (6)
				    AND pe.preid not in (SELECT preid FROM par.processoobrasparcomposicao where pocstatus = 'A' )
				    and pe.prevalorobra is not null
				    -- verificando se a obra não foi empenhado - solicitado pelo Daniel Areas -- 
				    and (select count(*) from par.empenhoobrapar where preid=pe.preid and eobstatus = 'A')=0
				order by 
					descricao";
		
		return $sql;
	}
	
	public function listaObrasEmendasPAR( $filtro ){
		//<a href=\"javascript:listarDados(\'' || pe.preid || '\', \'' || pe.preano || '\', \'' || iu.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Obra\" border=\"0\"></a>
		$sql = "SELECT DISTINCT
				    '<center><input type=checkbox name=chk[] id=chk_'||pe.preid||' value='||pe.preid||'_'||COALESCE(pe.prevalorobra, 0)||'></center>' as acoes,
				    pe.predescricao as descricao,
				    pto.ptodescricao,
					mun.mundescricao,
					mun.estuf,
					esd.esddsc,
				  	COALESCE(pe.prevalorobra, 0) as valor,
				    pe.preano||' ' as ano 
				FROM par.instrumentounidade 	iu
				    INNER JOIN par.pontuacao 	po on po.inuid  = iu.inuid AND ptostatus <> 'I'
				    INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid
				    INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'					
				    inner join par.subacaoobra	sob on sob.sbaid 	= s.sbaid
				    inner join obras.preobra	pe on pe.preid	= sob.preid and pe.prestatus = 'A' 
				    INNER JOIN obras.pretipoobra pto ON pto.ptoid = pe.ptoid AND pto.ptostatus = 'A' 
				    INNER JOIN territorios.municipio mun ON mun.muncod = pe.muncod 
					INNER JOIN workflow.documento doc ON doc.docid = pe.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				WHERE
				    iu.inuid = {$_SESSION['par']['inuid']}  
				    AND s.frmid IN ( $filtro ) 
				    --AND pto.tooid = 4
				    AND esd.esdid IN(".WF_PAR_OBRA_APROVADA.",".WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL.")
				    AND pe.preid not in (SELECT preid FROM par.processoobrasparcomposicao where pocstatus = 'A' )
				    and pe.prevalorobra is not null
				    -- verificando se a obra não foi empenhado - solicitado pelo Daniel Areas -- 
				    and (select count(*) from par.empenhoobrapar where preid=pe.preid and eobstatus = 'A')=0
				order by 
					pe.predescricao";
		
		return $sql;
	}
	
	public function listaObrasPAC( $tooid, $classsificacao, $ptoid = null ){
		
		$estuf	= $_SESSION['par']['estuf'];
		$muncod	= $_SESSION['par']['muncod'];
		
		if( $muncod ){
			$filtro = "AND muncodpar = '$muncod'
					   AND pto.ptoesfera in ('M','T')";
					   
		} else if( $estuf ){
			$filtro = "AND pre.estufpar = '$estuf' 
						AND (pre.muncodpar IS NULL or pre.muncodpar = '')
						AND pto.ptoesfera in ('E','T')";
		}
		
		if($estuf == 'DF' || $muncod == '5300108' ){
			$filtro = "AND pre.estufpar = '$estuf'
					   AND pto.ptoesfera in ('M','T')";
		}
		
		if( $ptoid ){
			$filtroptoid = "AND pto.ptoid in (".$ptoid.")";
		} else {
			$filtroptoid = "AND pto.ptoid not in (44,45,43,42)";
		}
		
		//<a href=\"javascript:listarDados(\'' || pre.preid || '\', \'' || pre.preano || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Obra\" border=\"0\"></a>
		
		$sql = "SELECT
					'<center><input type=checkbox onclick=marcarChk(this); name=chk[] id=chk_'||pre.preid||' value='||pre.preid||'_'||COALESCE(pre.prevalorobra, 0)||'_'||pto.ptoclassificacaoobra||'></center>' as acoes,
				    pre.predescricao as descricao,
				    case when pto.ptoclassificacaoobra = 'Q' then 'Quadra'
						when pto.ptoclassificacaoobra = 'C' then 'Cobertura'
					else 'Proinfância' end as tipo,
					mun.mundescricao,
					mun.estuf,
					esd.esddsc,
				  	COALESCE(pre.prevalorobra, 0) as valor,
				    pre.preano||' ' as ano
				FROM obras.preobra pre
					INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
					left JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
					INNER JOIN workflow.documento                       doc ON doc.docid = pre.docid
					INNER JOIN workflow.estadodocumento             esd ON esd.esdid = doc.esdid
				WHERE esd.esdid in ( ".WF_TIPO_OBRA_APROVADA.",".WF_TIPO_OBRA_APROVACAO_CONDICIONAL." )
					AND pto.tooid in (".$tooid.") 
					$filtroptoid 
					AND pto.ptoclassificacaoobra IN ('".$classsificacao."')
					AND ptostatus = 'A'
					AND pre.preid not in (SELECT preid FROM par.processoobraspaccomposicao where pocstatus = 'A')
					$filtro
				order by 
					pre.predescricao";
		//ver($sql);
		return $sql;
	}
	
	public function listaHistoricoProcesso(){
		global $db;
		
		$acBotaoPAR = "'<input type=\"button\" name=\"solicitarconta\" id=\"solicitarconta\" value=\"Solicitar Conta Corrente\" onclick=\"solicitarContaCorrente(\''||prp.prpid||'\', ".$this->subacoes_generico_par.");\">'";
		$acoesPAR	= "'<img style=\"cursor:pointer\" id=\"img_dimensao_'|| prp.prpid ||'\" src=\"/imagens/mais.gif\" onclick=\"carregarListaProcesso(this.id,'|| prp.prpid ||', ".$this->subacoes_generico_par.");\" border=\"0\">&nbsp;&nbsp;&nbsp;'";
		
		$acBotaoObrasPAR = "'<input type=\"button\" name=\"solicitarconta\" id=\"solicitarconta\" value=\"Solicitar Conta Corrente\" onclick=\"solicitarContaCorrente(\''||prp.proid||'\', ".$this->obras_par.");\">'";
		$acoesObrasPAR	 = "'<img style=\"cursor:pointer\" id=\"img_dimensao_'|| prp.proid ||'\" src=\"/imagens/mais.gif\" onclick=\"carregarListaProcesso(this.id,'|| prp.proid ||', ".$this->obras_par.");\" border=\"0\">&nbsp;&nbsp;&nbsp;'";
		
		$acBotaoPAC = "'<input type=\"button\" name=\"solicitarconta\" id=\"solicitarconta\" value=\"Solicitar Conta Corrente\" onclick=\"solicitarContaCorrente(\''||prp.proid||'\', ".$this->obras_pac_quadra.");\">'";
		$acoesPAC	= "'<img style=\"cursor:pointer\" id=\"img_dimensao_'|| prp.proid ||'\" src=\"/imagens/mais.gif\" onclick=\"carregarListaProcesso(this.id,'|| prp.proid ||', ".$this->obras_pac_quadra.");\" border=\"0\">&nbsp;&nbsp;&nbsp;'";
		
		$estuf	= $_SESSION['par']['estuf'];
		$muncod	= $_SESSION['par']['muncod'];
		
		if( $estuf && empty($muncod) ){
			$filtro = "AND pre.estufpar = '$estuf' 
						AND pre.muncodpar IS NULL
						AND pto.ptoesfera in ('E','T')";
		} else if( $muncod ){
			$filtro = "AND muncodpar = '$muncod'
					   AND pto.ptoesfera in ('M','T')";
		}
		
		$sql = "select 
					acoes,
				    processo,
				    agencia,
				    banco,
				    data,
				    tipo,
				    div
				from(
				SELECT DISTINCT
				    case when prp.prpseqconta is null then 
				    	$acoesPAR||$acBotaoPAR
				    else $acoesPAR end as acoes,
				    prp.prpnumeroprocesso||' ' as processo, 
				    prp.prpagencia||' ' as agencia, 
				    prp.prpbanco||' ' as banco, 
				    to_char(prp.prpdatainclusao, 'DD/MM/YYYY') as data,
				    case when prptipo = 'B' then 
				    	'Subações do Brasil-Pró' 
				    else 
						case when prp.sisid = 57 then 'Subações de Emendas no PAR' else 'Subações Genérico do PAR' end
					end as tipo,
				    '</td></tr>
				                <tr style=\"display: none\" id=\"listaSubacaoProcesso_' || prp.prpid ||'\" >
				                    <td id=\"trV_' || prp.prpid ||'\" colspan=8 ></td>
				            </tr>'  as div  
				FROM par.processopar prp
				    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
				WHERE
				    prp.inuid = {$_SESSION['par']['inuid']}
				    and prp.prpstatus = 'A'
				UNION
				SELECT DISTINCT
				    case when prp.proseqconta is null then 
				    	$acoesObrasPAR||$acBotaoObrasPAR
				    else $acoesObrasPAR end as acoes,
				    prp.pronumeroprocesso||' ' as processo, 
				    prp.proagencia||' ' as agencia, 
				    prp.probanco||' ' as banco, 
				    to_char(prp.prodatainclusao, 'DD/MM/YYYY') as data,
				    case when prp.protipo = 'B' then 
				    	'Obras do PAR - Brasil-Pró' 
				    else 
						'Obras do PAR'
					end as tipo,
				    '</td></tr>
				                <tr style=\"display: none\" id=\"listaSubacaoProcesso_' || prp.proid ||'\" >
				                    <td id=\"trV_' || prp.proid ||'\" colspan=8 ></td>
				            </tr>'  as div  
				FROM par.processoobraspar prp
				    inner join par.processoobrasparcomposicao ppc on ppc.proid = prp.proid and ppc.pocstatus = 'A'
				WHERE
				    prp.inuid = {$_SESSION['par']['inuid']}
				    AND prp.prostatus = 'A'
				UNION
				SELECT DISTINCT
				    case when prp.proseqconta is null then 
				    	$acoesPAC||$acBotaoPAC
				    else $acoesPAC end as acoes,
				    prp.pronumeroprocesso||' ' as processo, 
				    prp.proagencia||' ' as agencia, 
				    prp.probanco||' ' as banco, 
				    to_char(prp.prodatainclusao, 'DD/MM/YYYY') as data,
				    'Obras do PAC' as tipo,
				    '</td></tr>
				                <tr style=\"display: none\" id=\"listaSubacaoProcesso_' || prp.proid ||'\" >
				                    <td id=\"trV_' || prp.proid ||'\" colspan=8 ></td>
				            </tr>'  as div  
				FROM par.processoobra prp
				    inner join par.processoobraspaccomposicao 	ppc	on ppc.proid = prp.proid and ppc.pocstatus = 'A'
				    inner join obras.preobra					pre	on pre.preid = ppc.preid
					inner join obras.pretipoobra 				pto on pre.ptoid = pto.ptoid
				WHERE
					1=1
					and prp.prostatus = 'A'
				    $filtro
				) as foo";
				
		$cabecalho = array("Ações", "Processo", "Agência", "Banco", "Data Inclusão", "Tipo");
		$db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'N');		
	}
	
	public function listaHistoricoProcessoPorTipo( $post ){
		global $db;
		
		switch ($post['tipo']) {
			case $this->subacoes_generico_par:
			case $this->subacoes_brasil_pro:
				$sql = $this->listaHistoricoProcessoPar($_POST['codigo']);
			break;
			case $this->subacoes_par_emendas:
				$sql = $this->listaHistoricoProcessoPar($_POST['codigo']);
			break;
			case $this->obras_par_brasil_pro:
			case $this->obras_par:
				$sql = $this->listaHistoricoProcessoObrasPar($_POST['codigo']);
			break;
			case $this->obras_pac_quadra:
				$sql = $this->listaHistoricoProcessoPac($_POST['codigo']);
			break;
		}
		
		$cabecalho = array("Descrição", "Ano", "Valor");
		$db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'N');
	}
	
	public function listaHistoricoProcessoPar($codigo){
		
		$sql = "SELECT DISTINCT
					s.sbadsc as descricao,
				    sd.sbdano as ano,
				    (SELECT cast(par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano) as numeric(20,2))) AS valor
				FROM par.processopar p
					inner join par.processoparcomposicao pc ON pc.prpid = p.prpid and pc.ppcstatus = 'A'
					inner join par.subacaodetalhe sd        ON sd.sbdid = pc.sbdid
				    inner join par.subacao s                ON s.sbaid = sd.sbaid
				WHERE
					p.inuid = {$_SESSION['par']['inuid']}
				    and p.prpid = $codigo
				    and p.prpstatus = 'A'
				ORDER BY descricao";
		
		return $sql;
	}
	
	public function listaHistoricoProcessoObrasPar($codigo){
		
		$sql = "SELECT distinct
					pe.predescricao as descricao,
				    pe.preano||' ' as ano,
				    COALESCE(pe.prevalorobra, 0) as valor
				FROM par.processoobraspar pro
					inner join par.processoobrasparcomposicao	ppc	on ppc.proid = pro.proid and ppc.pocstatus = 'A'
				    inner join obras.preobra					pe 	on pe.preid	= ppc.preid and pe.prestatus = 'A'
				WHERE
				    pro.inuid = {$_SESSION['par']['inuid']}
				    and pro.prostatus = 'A'
				    and pro.proid = $codigo
				ORDER BY descricao";
				
		return $sql;
	}
	
	public function listaHistoricoProcessoPac($codigo){
		
		$estuf	= $_SESSION['par']['estuf'];
		$muncod	= $_SESSION['par']['muncod'];
		
		if( $estuf ){
			$filtro = "AND pre.estufpar = '$estuf' 
						AND pre.muncodpar IS NULL
						AND pto.ptoesfera in ('E','T')";
		} else if( $muncod ){
			$filtro = "AND muncodpar = '$muncod'
					   AND pto.ptoesfera in ('M','T')";
		}
		
		$sql = "SELECT
					pre.predescricao as descricao,
				    pre.preano||' ' as ano,
				    COALESCE(pre.prevalorobra, 0) as valor
				FROM par.processoobra po
					inner join par.processoobraspaccomposicao	ppc	on ppc.proid = po.proid and ppc.pocstatus = 'A'
				    inner join obras.preobra 					pre	on pre.preid = ppc.preid
				    INNER JOIN obras.pretipoobra 				pto on pre.ptoid = pto.ptoid
				    INNER JOIN workflow.documento               doc on doc.docid = pre.docid
				    INNER JOIN workflow.estadodocumento         esd on esd.esdid = doc.esdid
				WHERE
					po.proid = $codigo
					and po.prostatus = 'A'
					$filtro
				    and esd.esdid = 228
				    and pto.tooid = 1
				    and ptostatus = 'A'";
		
		return $sql;
	}

	public function criaAbaPar( $tiposAba = 1 ){
		if( $tiposAba == 1){
			$abasPar = array( 0 => array( "descricao" => "Administrar Processo", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=administrarProjeto"),
							  1 => array( "descricao" => "Criar Processo", 			"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=montarProjeto")
							 );
		}elseif( $tiposAba == 2){
			$abasPar = array( 0 => array( "descricao" => "Administrar Processo", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=administrarProjeto"),
							  1 => array( "descricao" => "Reformular Projeto", 		"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid={$_REQUEST['prpid']}"),
							  2 => array( "descricao" => "Criar Processo", 			"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=montarProjeto")
							 );
		}elseif( $tiposAba == 3){
			$abasPar = array( 0 => array( "descricao" => "Administrar Processo", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=administrarProjeto"),
							  1 => array( "descricao" => "Reformular Projeto", 		"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid={$_REQUEST['prpid']}"),
							  2 => array( "descricao" => "Historico Reformulacao", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=historicoReformulacao"),
							  3 => array( "descricao" => "Criar Processo", 			"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=montarProjeto")
							 );
		}elseif( $tiposAba == 4 ){
			$abasPar = array( 0 => array( "descricao" => "Administrar Processo", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=administrarProjeto"),
							  1 => array( "descricao" => "Reformular Projeto", 		"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObrasPar&proid={$_REQUEST['proid']}"),
							  2 => array( "descricao" => "Criar Processo", 			"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=montarProjeto")
							 );
		}elseif( $tiposAba == 5 ){
			$abasPar = array( 0 => array( "descricao" => "Administrar Processo", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=administrarProjeto"),
							  1 => array( "descricao" => "Reformular Projeto", 		"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObrasPar&proid={$_REQUEST['proid']}"),
							  2 => array( "descricao" => "Historico Reformulacao", 	"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=historicoReformulacaoObraPar"),
							  3 => array( "descricao" => "Criar Processo", 			"link"	  => "par.php?modulo=principal/projetos&acao=A&abas=montarProjeto")
							 );
		}
		
		return $abasPar;
	}

	public function cancelarProcesso(){
		
		//if( $this->tipoProcesso == $this->subacoes_generico_par ){ #PAR AQUISICAO
		
		switch ( $this->tipoProcesso ) {
			case $this->subacoes_generico_par:
				$this->cancelarProcessoPAR();
			break;
			case 'obra':
				$this->cancelarProcessoObrasPAR();
			break;
			case 'pac':
				$this->cancelarProcessoObrasPAC();
			break;
		}
	}
	
	public function cancelarProcessoPAR(){
		global $db;
		
		$arrParamProcesso = array(
								'an_processo' 			=> date("Y"),
						    	'nu_processo' 			=> $db->pegaUm("select prpnumeroprocesso from par.processopar where prpstatus = 'A' and prpid = {$this->arrPost['codigo']}"),
						    	'tp_processo' 			=> $this->tpProcesso,
						    	'co_programa_fnde' 		=> $this->programafnde,
								'nu_sistema'			=> $this->tipnumerosistemasigef,
								'nu_cnpj_favorecido' 	=> $this->cnpjEntidade,
								'ds_resumo' 			=> 'Manutenção e desenvolvimento da educação'
							);
		
		$obProcesso = new ProcessoFNDE( $this->wsusuario, $this->wssenha );
		$arrProcesso = $obProcesso->cancelarProcesso( $arrParamProcesso );		
	}
	
	public function cancelarProcessoObrasPAR(){
		global $db;		
		
	}
	
	public function cancelarProcessoObrasPAC(){
		global $db;		
		
	}
}
?>