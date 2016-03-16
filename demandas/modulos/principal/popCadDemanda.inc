<?php
/*
 * Função criada com a finalidade de filtrar e montar <select> 
 * Utilizada no momento, por requisição ajax
 */

function montaCelula($ordid){
	global $db;
 
	if($ordid == 1 || $ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21){
		
		$ordidx1 = 1;
		
		$sql = "SELECT
				 c.celid AS codigo,
				 c.celnome || ' - Gerente: ' || 
				 CASE 
				 	 WHEN u.usunome != '' THEN u.usunome
					 ELSE ' - '
				 END as DESCRICAO
				FROM
				 demandas.celula c
	 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
	 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
				WHERE
				 c.ordid = ".$ordidx1." AND
				 c.celstatus = 'A'
				ORDER BY
				 c.celnome;";
		$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --','filtraSistema', '' );
		echo obrigatorio();
		
	}
	else if($ordid == 2 || $ordid == 13 || $ordid == 14 || $ordid == 23 || $ordid == 24){
		$sql = "SELECT
				 c.celid AS codigo,
				 c.celnome as DESCRICAO
				FROM
				 demandas.celula c
				WHERE
				 c.ordid = ".$ordid." AND
				 c.celstatus = 'A'
				ORDER BY
				 c.celnome;";
		$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --','filtraTipoRede', '' );
		echo obrigatorio();
		
	}
		
}


function montaSistema($celid){
	global $db;
 
	$sql = sprintf("select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = %d
					order by s.sidabrev", 
			$celid);
	$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o Sistema --', '', '' );
	echo obrigatorio();
	//listar_municipios 
}


function montaTipoRede($celid){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipid AS codigo,
					 tipnome AS descricao 
					FROM 
					 demandas.tiposervico
					WHERE
					 celid = %d AND
					 tipstatus = 'A' 
					ORDER BY 
					 tipnome", 
			$celid);
	$db->monta_combo( 'tipid', $sql, 'S', '-- Informe o tipo da demanda --', 'filtraQtd', '' );
	echo obrigatorio();
	//listar_municipios 
}


function montaTipoDemanda($ordid){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipid AS codigo,
					 tipnome AS descricao 
					FROM 
					 demandas.tiposervico
					WHERE
					 ordid = %d AND
					 tipstatus = 'A' 
					ORDER BY 
					 tipnome", 
			$ordid);
	$db->monta_combo( 'tipid', $sql, 'S', '-- Informe o tipo da demanda --', 'filtraQtd', '' );
	echo obrigatorio();
	//listar_municipios 
}


function montaQtdDemanda($tipid){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipquantidade 
					FROM 
					 demandas.tiposervico
					WHERE
					 tipid = %d AND
					 tipstatus = 'A' 
					", 
			$tipid);
	$qtd = $db->PegaUm($sql);
	
	if($qtd == 't'){
		echo campo_texto( 'dmdqtde', 'S', 'S', '', 10, 8, '########', '','','','','id="dmdqtde"','','1');
	}
	else echo "N";
	 
}

/*
 * Função que faz o insert da demanda 
 */
function salvaDemanda(){
	global $db;
	
	$select = array();
	$dado	= array();
	
	if ($_POST['outrouser'] == 1){
		//$select[0] = "unaid";
		//$dado[0]   = $_POST['unaid'];	
		$select[0] = 'dmdnomedemandante';
		$dado[0]   = $_POST['usudemandante'];
		$select[1] = "dmdemaildemandante";
		$dado[1]   = $_POST['usermail'];	
		$select[2] = "usucpfdemandante";
		if($_POST['usucpf']){
			$dado[2]   = $_POST['usucpf'];
			$usucpf = $_POST['usucpf'];
		}else{
			$dado[2]   = $_SESSION['usucpf'];
			$usucpf = $_SESSION['usucpf'];
		}	
	}else{
		$select[0] = "usucpfdemandante";
		$dado[0]   = $_POST['usucpf'];	
		$usucpf = $_POST['usucpf'];
	}
	
	
	if($_POST['horarioA'] && $_POST['horarioN']){
		$horario = "T";	
	}elseif($_POST['horarioA'] && !$_POST['horarioN']){
		$horario = "A";
	}elseif(!$_POST['horarioA'] && $_POST['horarioN']){
		$horario = "N";
	}else{
		$horario = "C";
	}
	
	
	if(!$_POST['atendimentoRemoto']) $_POST['atendimentoRemoto'] = 'f';
	if(!$_POST['dmdatendurgente']) $_POST['dmdatendurgente'] = 'f';
	
	if(!$_POST['dmdqtde']) $_POST['dmdqtde'] = '1';

	if($_POST['tipid']==0) $_POST['tipid']="";
	
							   
	$sql = sprintf("INSERT INTO demandas.demanda
					(
						%s
						usucpfinclusao,
						tipid,
						sidid,
						dmdtitulo,
						dmddsc,
						dmdstatus,
						laaid,
						dmdsalaatendimento,
						unaid,
						dmdidorigem,
						dmdqtde,
						dmdhorarioatendimento,
						celid,
						dmddatainclusao,
						usucpfanalise,
						dmdatendremoto,
						dmdatendurgente,
						dmdjusturgente,
						usucpfclassificador,
						dmddataclassificacao			
					)VALUES (
						%s
						'%s',
						%d,
						%s,
						'%s',
						'%s',
						'A',
						%s,
						'%s',
						%d,
						%d,
						%d,
						'%s',
						%s,
						'%s',
						'%s',
						'%s',
						'%s',
						'%s',
						'%s',
						'%s'
					) RETURNING dmdid",
					(count($dado) > 0 ? implode(',',$select)."," : ''),
					(count($dado) > 0 ? "'".implode("','",$dado)."'," : ''), 
					$_SESSION['usucpf'],
					$_POST['tipid'] ? $_POST['tipid'] : 'null',
					($_POST['sidid'] ? $_POST['sidid'] : 'null'),
					($_POST['assunto'] ? simec_addslashes($_POST['assunto']) : ''),
					($_POST['necessidade'] ? simec_addslashes($_POST['necessidade']) : ''),
					($_POST['laaid'] ? $_POST['laaid'] : 'null'),
					$_POST['dmdsalaatendimento'],
					$_POST['unaid'],
					$_POST['dmdidorigem'],
					$_POST['dmdqtde'],
					$horario,
					($_POST['celid'] ? $_POST['celid'] : 'null'),
					date('Y-m-d H:i:s'),
					$_SESSION['usucpf'],
					$_POST['atendimentoRemoto'],
					$_POST['dmdatendurgente'],
					$_POST['dmdjusturgente'],
					$_SESSION['usucpf'],
					date('Y-m-d H:i:s')
					);
	//dbg($sql,1);
	$subdmdid = $db->pegaUm($sql);


	if(!$subdmdid){
		die('<script>
	 			alert("Problemas ao cadastrar a subdemanda. Favor avisar ao Gestor do sistema.");
	 			self.close();
	 		</script>');
	}
	
	//Insere nº de serie / patrimonio
	if($_POST['nserie'] || $_POST['npatrimonio']){
		$sql = sprintf("INSERT INTO demandas.itemhardwaredemanda
					(
						dmdid,
						ihdnumserie,
						ihdnumpatrimonio,
						ihdstatus		
					)VALUES(
						'%s',
						'%s',
						'%s',
						'%s'
					)",
					$subdmdid,
					$_POST['nserie'],
					$_POST['npatrimonio'],
					'A'
					);
		$db->executar($sql); 
	}
	
	
	$db->commit();
	//echo $sql;
	//exit;
	
	
	//envia email p/ administradores para suporte de atendimento
	
	// Pega informações da demanda
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u.usunome AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
			 dmdid = {$subdmdid}";	

	$dado 		  = (array) $db->pegaLinha($sql);
	
	//origem da demanda diferente de sistema de informação
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
	
			// Seta remetente
			$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);
			
			// Seta Destinatário 
			$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$subdmdid}";
			$dadox = (array) $db->carregarColuna($sqlx);
			$gerente = implode("; ", $dadox);
			
			if($gerente){
			
					$destinatario = $gerente;
					
					if($dado['codorigem'] == '8') $emailCopia   = "sqladm@mec.gov.br";
					
					$ano		  = $dado['ano'];
				
					// seta dados da demanda
					$dadoDemanda = array();
					$dadoDemanda['ID'] 				  = $dado['dmdid'];
					$dadoDemanda['Origem da demanda'] = $dado['origem'];
				
					if ($dado['sistema'])
					$dadoDemanda['Sistema']		  = $dado['sistema'];
				
					$dadoDemanda['Assunto']			  = $dado['assunto'];
					$dadoDemanda['Descricão']		  = $dado['descricao'];
					$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');
				
				
					// seta dados do demandante
					$dadoDemandante = array (
				    						 "Solicitante" => $dado['demandante'],
				    						 "Telefone"    => $dado['fone'],
				     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
				    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
				    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
				    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
					);
					// Busca arquivos
					$sql = "SELECT
								arqnome||'.'||arqextensao AS arquivo,
								to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
							FROM 
								demandas.anexos a
								INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
							WHERE	
								a.dmdid = {$subdmdid}"; 
				
					$dadoArquivo = $db->carregar($sql);
					$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
																		array(
																			  "arquivo" => '-',
																			  "data"	=> '-'	 
																			  )
																	  );
				  // Busca observações
				  $sql = "SELECT
								obsdsc AS descricao,
								usunome AS usuario
							FROM 
								demandas.observacoes o
								INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
							WHERE	
								o.obsstatus = 'A' AND
								dmdid = {$subdmdid}"; 		
				
							  $dadoObs = $db->carregar($sql);
							  $dadoObs = $dadoObs ? $dadoObs : array(
							  array(
				  "descricao" => '-',
				  "usuario"	=> '-'	 
				  )
				  );
				
				  // Seta assunto
				  $assunto = "Abertura de SubDemanda –[{$subdmdid}/{$ano}]";
				
				  // Seta Conteúdo
				  $conteudo = textMail("Abertura de SubDemanda [{$subdmdid}]", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

				  //envia email
				  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
			}
	}	
	
}


/*
 * Executa a função que filtra e retorna o <select>
 */
if ($_REQUEST['ordidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaTipoDemanda($_POST['ordidAjax']);
	exit;
}/*elseif ($_REQUEST['unaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaUsuario($_POST['unaidAjax']);
	exit;	
}*/

if ($_REQUEST['ordidAjax2']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaCelula($_POST['ordidAjax2']);
	exit;
}

if ($_REQUEST['celtiporedeAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaTipoRede($_POST['celtiporedeAjax']);
	exit;
}


//filtra tipo para mostrar qtd
if ($_REQUEST['tipidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaQtdDemanda($_POST['tipidAjax']);
	exit;
}


//filtra o sistema
if ($_REQUEST['celidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSistema($_POST['celidAjax']);
	exit;
}

/*
 * Executa a função que salva a demanda
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_POST['varaux'] == 'okCad'){
 	salvaDemanda();
 	
	die("<script>
			window.opener.location='demandas.php?modulo=principal/listaDemandasRelacionadas&acao=A';
			alert('Operação realizada com sucesso!');
			location.href='demandas.php?modulo=principal/popCadDemanda&acao=A&dmdid=".$_POST['dmdidorigem']."';
			//history.go(-1);
		 </script>");
}





//recupera dados
if($_REQUEST['usucpf']){
	$sqlx = "SELECT d.dmdid,
		d.dmdtitulo,
		o.ordid,
		o.orddescricao,
		t.tipid,
		s.sidid,
		s.siddescricao,
		t.tipnome,
		d.dmddsc,
		d.dmdreproducao,
		d.dmdsalaatendimento,
		d.laaid,
		l.lcaid,
		d.unaid,
		ua.unasigla,
		ua.unadescricao,
		d.usucpfdemandante,
		u.usdramal,
		u.usdcelular,
		u.usdgabinete,
		u.usdsala,
		u.unaid as unaid2,
		u.laaid as laaid2,
		l2.lcaid as lcaid2
	
		FROM demandas.demanda AS d
	
	LEFT JOIN  demandas.unidadeatendimento AS ua ON ua.unaid = d.unaid 
	LEFT JOIN demandas.tiposervico AS t ON t.tipid = d.tipid 
	LEFT JOIN demandas.origemdemanda AS o ON o.ordid = t.ordid 
	LEFT JOIN demandas.sistemadetalhe AS s ON s.sidid = d.sidid 
	LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
	LEFT JOIN demandas.usuariodetalhes AS u ON u.usucpf = d.usucpfdemandante 
	LEFT JOIN demandas.localandaratendimento AS l2 ON l2.laaid = u.laaid
	WHERE d.usucpfdemandante = '".$_REQUEST['usucpf']."' order by d.dmdid desc limit 1";
	
}else{
	$sqlx = "SELECT d.dmdid,
		d.dmdtitulo,
		o.ordid,
		o.orddescricao,
		t.tipid,
		s.sidid,
		s.siddescricao,
		t.tipnome,
		d.dmddsc,
		d.dmdreproducao,
		d.dmdsalaatendimento,
		d.laaid,
		l.lcaid,
		d.unaid,
		ua.unasigla,
		ua.unadescricao,
		d.usucpfdemandante,
		u.usdramal,
		u.usdcelular,
		u.usdgabinete,
		u.usdsala,
		u.unaid as unaid2,
		u.laaid as laaid2,
		l2.lcaid as lcaid2
	
		FROM demandas.demanda AS d
	
	LEFT JOIN  demandas.unidadeatendimento AS ua ON ua.unaid = d.unaid 
	LEFT JOIN demandas.tiposervico AS t ON t.tipid = d.tipid 
	LEFT JOIN demandas.origemdemanda AS o ON o.ordid = t.ordid 
	LEFT JOIN demandas.sistemadetalhe AS s ON s.sidid = d.sidid 
	LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
	LEFT JOIN demandas.usuariodetalhes AS u ON u.usucpf = d.usucpfdemandante 
	LEFT JOIN demandas.localandaratendimento AS l2 ON l2.laaid = u.laaid
	WHERE d.dmdid = ".$_REQUEST['dmdid'];
}
	
	$dadosx = $db->carregar($sqlx);
	
	
	
	$unasigla = $dadosx[0]['unasigla'];
	$unadescricao = $dadosx[0]['unadescricao'];
	$orddescricao = $dadosx[0]['orddescricao'];
	$dmdtitulo = $dadosx[0]['dmdtitulo'];
	
	$usdgabinete =  $dadosx[0]['usdgabinete'];
	$usdramal =  $dadosx[0]['usdramal'];
	$usdcelular =  formata_fone_fax($dadosx[0]['usdcelular']);
	
	//setor
	if($dadosx[0]['unaid2']){ 
		$unaid =  $dadosx[0]['unaid2'];
	}
	else{
		$unaid =  $dadosx[0]['unaid'];
	}
	//local
	if($dadosx[0]['lcaid2']){ 
		$lcaid =  $dadosx[0]['lcaid2'];
	}
	else{
		$lcaid =  $dadosx[0]['lcaid'];
	}
	//andar
	if($dadosx[0]['laaid2']){ 
		$laaid =  $dadosx[0]['laaid2'];
	}
	else{
		$laaid =  $dadosx[0]['laaid'];
	}
	//sala
	if($dadosx[0]['usdsala']){ 
		$dmdsalaatendimento =  $dadosx[0]['usdsala'];
	}
	else{
		$dmdsalaatendimento =  $dadosx[0]['dmdsalaatendimento'];
	}
	
	
	$usucpf = $_REQUEST['usucpf'] ? $_REQUEST['usucpf'] : $dadosx[0]['usucpfdemandante'];
	if(!$usucpf) $usucpf = $_SESSION['usucpf'];
	
	
//fim recupera dados	

//include APPRAIZ ."includes/cabecalho.inc";
global $db;
print '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de SubDemanda - Apartir do Cód. # '.$_REQUEST['dmdid'], '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0">

<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="formDemanda" action="" method="post" enctype="multipart/form-data" onsubmit="return validaForm();">

<!-- <input type="hidden" name="unaid" id="unaid" value=""> -->
<input type="hidden" name="dmdidorigem" id="dmdidorigem" value="<?=$_REQUEST['dmdid']; ?>">
<input type="hidden" name="outrouser" id="outrouser" value="0">
<input type="hidden" name="usucpf" id="usucpf" value="<?=$usucpf;?>">
<input type="hidden" name="usermail" id="usermail" value="">
<input type="hidden" name="verificaqtd" id="verificaqtd" value="">
<input type="hidden" name="varaux" id="varaux" value="">

<table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr id="tr_user2">
		<td class="SubTituloDireita" style="width:39.5%;">Demandante:</td>
		<?
		/*
		if($_REQUEST['dmdid']){
			
			$sql = "SELECT	usucpfdemandante FROM demandas.demanda WHERE dmdid = ".$_REQUEST['dmdid'];
			$usucpfdemandante = $db->pegaUm($sql);
					
			$usudemandante = nomeUser($usucpfdemandante);
			?>
			<script>
				document.getElementById('usucpf').value = '<?=$usucpfdemandante?>';
			</script>
			<?
		}
		else{
			$usudemandante = nomeUser($_SESSION['usucpf']);
		}
		*/
		$usudemandante = nomeUser($usucpf);
		?>
		<td><?= campo_texto( 'usudemandante', 'S', 'N', '', 40, 250, '', ''); ?>&nbsp;&nbsp;<a href="javascript:void(0);" onclick="popupPesqUser()" title="Cadastrar demanda em nome de outro usuário">Outro</a></td>
	</tr>	

	<tr>
		<td class="SubTituloDireita" >Demanda Origem:</td>
		<td><?=strtoupper($orddescricao.' - '.$dmdtitulo)?></td>
	</tr>	
	<tr>
		<td align='right' width="40%" class="SubTituloDireita">Qual é a <b>demanda</B>?:</td>
		<td>
			<table width="100%">
			<? $sql = "SELECT
						'<input type=\'radio\' name=\'ordid\' value=\'' || ordid || '\' onclick=\'filtraTipo('|| ordid ||');\'/>' || orddescricao || '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'AS campo 
					   FROM
					    demandas.origemdemanda
					   WHERE
					    ordstatus = 'A'
						ORDER BY 
						 orddescricao";
				$radio = (array) $db->carregar($sql);
				
				foreach ($radio as $radio){
					//$a++;
					//echo $a == 1 || $a == 7 || $a == 13 ? '<TR>' : '';
					//echo  '<td>'.$radio['campo'].'</td>';
					//echo $a == 3 || $a == 9 || $a == 15 ? '</TR>' : '';
					echo ($a%3) == 0 ? '<TR>' : '';
					echo  '<td>'.$radio['campo'].'</td>';
					echo ($a+1%3) == 0 ? '</TR>' : '';
					$a++;
				}	
		    ?>
		    </table>
		</td>
	</tr>
	<tr id="tr_celula" style="display:none;">
		<td align='right' class="SubTituloDireita">A demanda é relacionada a qual <b>Célula</b>:</td>
		<td id="td_celula">
		<?php
		//listar_municipios;
		?>	
		</td>
	</tr>
	<tr id="tr_sistema" style="display:none;">
		<td  align='right' class="SubTituloDireita">A demanda é relacionada a qual <b>Sistema</b>:</td>
		<td id="listasistema">
		<?php
		
		$sql = "SELECT 
				 sidid AS codigo,
				 upper(sidabrev) ||' - '|| siddescricao AS descricao
				FROM 
				 demandas.sistemadetalhe
				WHERE
				 sidstatus = 'X'
				ORDER BY 
				 siddescricao";
		$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o sistema --','', '' );
		echo obrigatorio(); 
		?>	
		</td>
	</tr>
	<tr id="tr_tipo" style="display:none;">
		<td align='right' class="SubTituloDireita">Informe o <b>tipo da demanda</B>:</td>
		<td id='tipodemanda'>
		</td>
	</tr>	
	<tr id="tr_qtd" style="display:none;">
		<td align='right' class="SubTituloDireita">Informe a <b>quantidade do serviço</B>:</td>
		<td id='qtddemanda'>
		</td>
	</tr>	
	<tr id="tr_nserie" style="display:none;">
		<td align='right' class="SubTituloDireita">Informe o <b>nº de série</B>:</td>
		<td>
			<?= campo_texto( 'nserie', 'S', 'S', '', 30, 50, '', ''); ?>
		</td>
	</tr>	
	<tr id="tr_npatrimonio" style="display:none;">
		<td align='right' class="SubTituloDireita">Informe o <b>nº de patrimônio</B>:</td>
		<td>
			<?= campo_texto( 'npatrimonio', 'S', 'S', '', 30, 50, '', ''); ?>
		</td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita">Informe o <b>assunto</b> para a demanda:</td>
		<td><?= campo_texto( 'assunto', 'S', 'S', '', 80, 250, '', ''); ?></td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita"><b>Descreva</b> sua necessidade:</td>
		<td><?= campo_textarea('necessidade', 'S', 'S', '', 80, 5, 4000); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Horário Alternativo do atendimento:<br><b>(Opcional)</b></td>
		<td>
			<input type="checkbox" name="horarioA" value="A"> Horário de Almoço (12:00 às 14:00)
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="checkbox" name="horarioN" value="N"> Horário Noturno (18:00 às 22:00)
		</td>
	</tr>				
	<tr>
		<td align='right' class="SubTituloDireita">Deseja receber atendimento remoto:</td>
		<td>
			<input type="radio" name="atendimentoRemoto" value="t" > Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="atendimentoRemoto" value="f" checked> Não
		</td>
	</tr>				
	
	<tr>
		<td align='right' class="SubTituloDireita">Atendimento é urgente:</td>
		<td>
			<input type="radio" name="dmdatendurgente" value="t" onclick="atendUrgente();"> Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="dmdatendurgente" value="f" onclick="atendUrgente();" checked> Não
		</td>
	</tr>				
	<tr id="tr_atendurgente" style="display:none;">
		<td  align='right' class="SubTituloDireita">Informe a justificativa:</td>
		<td><?= campo_textarea('dmdjusturgente', 'S ', 'S', '', 80, 5, 4000); ?></td>
	</tr>
	
	<tr>
		<td align='left' bgcolor="#DCDCDC" colspan="2" style="padding-left:57px"><b>Local de atendimento</b>:</td>
	</tr>

	<tr>
		<td class="subtitulodireita">Setor:</td>
		<td>
			<?php
				echo $unasigla .' - '. $unadescricao;
			?>
			<input type='hidden' name='unaid' id='unaid' value='<?=$unaid?>'>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Edifício:</td>
		<td>
		<?php
		if($lcaid){
			$sql = "SELECT 
					 lcadescricao
					FROM 
					 demandas.localatendimento
					WHERE
					LCAID = ".$lcaid;
			$lcadescricao = $db->PegaUm($sql);
			echo $lcadescricao;
		} 
		?>	
		</td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita" >Andar:</td>
		<td >
			<?
			if($laaid){
				$sql = sprintf("SELECT 
								 aa.anddescricao 
								FROM 
								 	demandas.localandaratendimento laa
									inner join demandas.andaratendimento aa on laa.andid = aa.andid
								WHERE
								 laa.laaid = %d 
								", 
						$laaid);
				$anddescricao = $db->PegaUm($sql);
				echo $anddescricao;
			}
			?>
			<input type='hidden' name='laaid' id='laaid' value='<?=$laaid?>'>
		</td>
	</tr>				
	<tr >
		<td align='right' class="SubTituloDireita">Sala:</td>
		<td>
			<?= $dmdsalaatendimento ?>
			<input type='hidden' name='dmdsalaatendimento' id='dmdsalaatendimento' value='<?=$dmdsalaatendimento?>'>
		</td>
	</tr>				
	<tr>
		<td align='right' class="SubTituloDireita">Ramal:</td>
		<td>
			<?= $usdramal ?>
			<input type='hidden' name='usdramal' id='usdramal' value='<?=$usdramal?>'>
	</tr>		
	<?if($usdcelular){?>
	<tr>
		<td align='right' class="SubTituloDireita">Celular:<br><b>(Opcional)</b></td>
		<td>
			<?= $usdcelular ?>
			<input type='hidden' name='usdcelular' id='usdcelular' value='<?=$usdcelular?>'>
	</tr>		
	<?}?>
	
	<tr bgcolor="#C0C0C0">
		<td width="15%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		&nbsp;&nbsp;
		<input type="button" class="botao" name="btfechar" value="Fechar" onclick='self.close();'>
		</td>
	</tr>	
</table>
</form>
<script type="text/javascript">
d = document;


function atendUrgente(){

	tr  = d.getElementById('tr_atendurgente');
	
	if(d.getElementsByName('dmdatendurgente')[0].checked == true){
		tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	if(d.getElementsByName('dmdatendurgente')[1].checked == true){
		tr.style.display = 'none';
	}
}

function popupPesqUser(){

	if (d.getElementById('outrouser').value == 0){
		param = 'usucpf='+d.getElementById('usucpf').value;
	}else{
		//param = 'usunome='+d.getElementsByName('usudemandante')[0].value+'&unaid='+d.getElementById('unaid').value+'&usermail='+d.getElementById('usermail').value;
		param = 'usunome='+d.getElementsByName('usudemandante')[0].value+'&usermail='+d.getElementById('usermail').value;
	}

	w = window.open('?modulo=principal/popup/pesqUser&acao=A&'+param,'pesqUser','scrollbars=yes,location=no,toolbar=no,menubar=no,width=550,height=200,left=250,top=125'); 
	w.focus();	
}

/*
* Faz requisição via ajax
* Filtra o tipo de damanda, atravéz do parametro passado 'ordid'
*/
function filtraTipo(ordid) {
	
	//esconde a tr qtd
	d.getElementById('tr_qtd').style.display = 'none';
	
	tr 	   = d.getElementById('tr_tipo');	
	td     = d.getElementById('tipodemanda');	
	select = td.getElementsByTagName('select')[0];
	trSis  = d.getElementById('tr_sistema');
	tdSis     = d.getElementById('listasistema');
	trCel  = d.getElementById('tr_celula');
	tdCel  = d.getElementById('td_celula');

	trCel.style.display = 'none';
	trSis.style.display = 'none';
	tdCel.style.display = 'none';
	tdSis.innerHTML = "<select  name='sidid'  class='CampoEstilo'  style='width: auto'><option value=''>-- Informe o sistema --</option></select> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />";

	if(ordid == 1 || ordid == 18 || ordid == 19 || ordid == 20 || ordid == 21){ //sistemas de informação
		trCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		trSis.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	else if(ordid == 2 || ordid == 13 || ordid == 14 || ordid == 23 || ordid == 24){ //2=Redes - 13=Gestão Documentos CGI 
		trCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		
	}
	
	
	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select)
		select.disabled = true;
		
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/popCadDemanda&acao=A', {
							        method:     'post',
							        parameters: '&ordidAjax=' + ordid,
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
										filtraCelula(ordid);
							        }
							  });
}

function filtraTipoRede(celid) {
	
	//esconde a tr qtd
	d.getElementById('tr_qtd').style.display = 'none';
	
	tr 	   = d.getElementById('tr_tipo');	
	td     = d.getElementById('tipodemanda');	
	//select = td.getElementsByTagName('select')[0];

	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	//if (select)
		//select.disabled = true;
		
	if(celid){
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/popCadDemanda&acao=A', {
							        method:     'post',
							        parameters: '&celtiporedeAjax=' + celid,
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
							        }
							  });
	}
	else{
		td.innerHTML = '';
		tr.style.display = 'none';
	}
}


function filtraCelula(ordid) {


	if(ordid != 11){//DIFERENTE de Projeto Web
		
		tdCel  = d.getElementById('td_celula');
		select = td.getElementsByTagName('select')[0];
			
			//alert(ordid);
		
		// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
		if (select)
			//select.disabled = true;
			
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/popCadDemanda&acao=A', {
								        method:     'post',
								        parameters: '&ordidAjax2=' + ordid,
								        onComplete: function (res)
								        {		
								        	//alert(res.responseText);	
											tdCel.innerHTML = res.responseText;
											tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
								        }
								  });
	}else{//Projeto Web
		celid = 9;
		filtraSistema(celid);
	}		  
}


/*
* Faz requisição via ajax
* Filtra a qtd de damanda, atravéz do parametro passado 'ordid'
*/
function filtraQtd(tipid) {

	tr 	   = d.getElementById('tr_qtd');	
	td     = d.getElementById('qtddemanda');	
	verificaQtd = d.getElementById('verificaqtd');
	tr_nserie = d.getElementById('tr_nserie');
	tr_npatrimonio = d.getElementById('tr_npatrimonio');


	//suporte de atendimento - "29 - Chamados para garantia (fornecedores) - REMOTO"
	if(tipid == 683){
		tr_nserie.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		tr_npatrimonio.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	else{
		tr_nserie.style.display = 'none';
		tr_npatrimonio.style.display = 'none';
	}	
	
	if(tipid){
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/popCadDemanda&acao=A', {
							        method:     'post',
							        parameters: '&tipidAjax=' + tipid,
							        onComplete: function (res)
							        {			
							        	if(res.responseText != "N"){
											td.innerHTML = res.responseText;
											tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
											verificaQtd.value='1';
											if(tipid == 683) d.getElementById('dmdqtde').disabled = true;
										}
										else{
											td.innerHTML = '';
											tr.style.display = 'none';
											verificaQtd.value='';
										}
										
							        }
							  });
	}
	else{
		td.innerHTML = '';
		tr.style.display = 'none';
		verificaQtd.value='';
	}
							  
}


/*
* Faz requisição via ajax
* Filtra o tipo de damanda, atravéz do parametro passado 'celid'
*/
function filtraSistema(celid) {

	if(!celid) celid = 999999;
	
	tr 	   = d.getElementById('tr_sistema');	
	td     = d.getElementById('listasistema');	
	select = td.getElementsByTagName('select')[0];
	//trSis  = d.getElementById('tr_sistema');
	//trCel  = d.getElementById('tr_celula');

	//trSis.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';	
	//trCel.style.display = paramSystem == 1 ? (navigator.appName == 'Netscape' ? 'table-row' : 'block') : 'none';
	
	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select)
		select.disabled = true;
		
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/popCadDemanda&acao=A', {
							        method:     'post',
							        parameters: '&celidAjax=' + celid,
							        onComplete: function (res)
							        {	
							        	td.innerHTML = res.responseText;
										tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
							        }
							  });
	
}




/*
* Function destinada a validação do formulário
*/
function validaForm(){
	elementos = d.getElementsByName('ordid');
	var checado = '';
	
	for (i=0; i < elementos.length; i++){
		if (elementos[i].checked){
			checado = 1;

			if (elementos[i].value == 1 || elementos[i].value == 18 || elementos[i].value == 19 || elementos[i].value == 20 || elementos[i].value == 21){//Sistemas de Informação
				if (d.getElementsByName('celid')[0].value == ''){
					alert('O campo célula, deve ser preenchido!');
					d.getElementsByName('celid')[0].focus();
					return false;
				}
				if (d.getElementsByName('sidid')[0].value == ''){
					alert('O campo sistema, deve ser preenchido!');
					d.getElementsByName('sidid')[0].focus();
					return false;
				}
			}
			else if (elementos[i].value == 2 || elementos[i].value == 13 || elementos[i].value == 14){//2=Redes - 13=Gestão Documentos CGI 
				if (d.getElementsByName('celid')[0].value == ''){
					alert('O campo célula, deve ser preenchido!');
					d.getElementsByName('celid')[0].focus();
					return false;
				}
			}
			else if (elementos[i].value == 11){//Projeto Web
				if (d.getElementsByName('sidid')[0].value == ''){
					alert('O campo sistema, deve ser preenchido!');
					d.getElementsByName('sidid')[0].focus();
					return false;
				}
			}
			
			break;
			
		}
	}
	
	if (checado != 1){
		alert('O campo demanda, deve ser preenchido!');
		return false;
	}
	
	if (d.getElementsByName('tipid')[0].value == ''){
		d.getElementsByName('tipid')[0].focus();
		alert('O campo tipo da demanda, deve ser preenchido!');
		return false;
	}

	if(d.getElementById('verificaqtd').value != ""){
		if (d.getElementById('dmdqtde').value < 1){
			d.getElementById('dmdqtde').focus();
			alert('O campo quantidade do serviço, deve ser maior que zero!');
			return false;
		}
	}

	//683 = suporte de atendimento - "29 - Chamados para garantia (fornecedores) - REMOTO"
	if (d.getElementsByName('tipid')[0].value == '683'){
		if (d.getElementsByName('nserie')[0].value == '' && d.getElementsByName('npatrimonio')[0].value == ''){
			alert('O campo nº de série ou nº de patrimônio, deve ser preenchido!');
			d.getElementsByName('nserie')[0].focus();
			return false;
		}		
	}	
	
	
	if (d.getElementsByName('assunto')[0].value == ''){
		d.getElementsByName('assunto')[0].focus();
		d.getElementsByName('assunto')[0].select();
		alert('O campo assunto, deve ser preenchido!');
		return false;
	}

	if (d.getElementsByName('necessidade')[0].value == ''){
		d.getElementsByName('necessidade')[0].focus();
		d.getElementsByName('necessidade')[0].select();
		alert('O campo necessidade, é obrigatório!');
		return false;
	}

	if(d.getElementsByName('dmdatendurgente')[0].checked == true){
		if(d.getElementsByName('dmdjusturgente')[0].value == ''){
			alert('O campo Justificativa, deve ser preenchido!');
			d.getElementsByName('dmdjusturgente')[0].focus();
			return false;
		}		
	}

	d.getElementsByName('varaux')[0].value = 'okCad';
	

	return true;
	
	
}
</script>
</body>