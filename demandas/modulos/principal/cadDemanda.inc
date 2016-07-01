<?php
/*
 * Função criada com a finalidade de filtrar e montar <select>
 * Utilizada no momento, por requisição ajax
 */

function montaCelula($ordid){
	global $db;

	if($ordid == 1 || $ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21){

		$ordidx1 = 1;


		//filtra por celula do demandasnte avançado
		if ( in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, arrayPerfil()) && count(arrayPerfil())==1){
			$sql = "select
							ur.celid
					from demandas.usuarioresponsabilidade ur
					where rpustatus = 'A'
					and usucpf = '".$_SESSION['usucpf']."'
					and ur.celid is not null
					and pflcod not in (235) --(235 = perfil demandante)
					";
			$dadosCel = $db->carregarColuna($sql);

			if($dadosCel) $andCel = " AND ur.celid in (".implode(",", $dadosCel).")";
		}

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
				 $andCel
				 --and c.celid not in (9) --celula web
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
					order by 2",
		$celid);
	$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o Sistema --', '', '' );
	echo obrigatorio();
	//listar_municipios
}

function limpaTelademanda(){



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
					 tipnome AS descricao,
                     tipdescricao as desc
					FROM
					 demandas.tiposervico
					WHERE
					 ordid = %d AND
					 tipstatus = 'A'
					ORDER BY
					 tipnome",
		$ordid);
	$dados  = $db->carregar( $sql );

	if($dados == false){
		echo 'Nenhum tipo da demanda encontrada para a origem da demanda selecionada';
		exit;
	}


	$db->monta_combo( 'tipid', $dados, 'S', '-- Informe o tipo da demanda --', 'filtraQtd', '' );
	echo obrigatorio();
	echo '<div id="container-desc">';
	foreach( $dados as $tipoServico ){

		$hidden = '<input type="hidden" ';
		$hidden .= 'name="tipoServico'. $tipoServico['codigo'] .'" ';
		$hidden .= 'id="tipoServico'. $tipoServico['codigo'] .'" ';
		$hidden .= 'value="'. nl2br(simec_htmlentities($tipoServico['desc'])) .'" />';

		echo $hidden;
	}
	echo '</div>';
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

	if($qtd == 't' || $qtd == 'true'){
		echo campo_texto( 'dmdqtde', 'S', 'S', '', 10, 8, '########', '','','','','id="dmdqtde"','','1');
	}
	else{
		echo "N";
	}
	exit;

}

function montaAndarDemanda($lcaid){
	global $db;

	$sql = sprintf("SELECT
					 laa.laaid AS codigo,
					 aa.anddescricao AS descricao
					 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao
					FROM
					 	demandas.localandaratendimento laa
						inner join demandas.andaratendimento aa on laa.andid = aa.andid
					WHERE
					 laa.lcaid = %d
					ORDER BY
					 aa.anddescricao",
		$lcaid);
	$dados = $db->carregar($sql);
	//echo 'ok='.count($dados).'<br>';
	if(count($dados)==1) $laaid = $dados[0]['codigo'];
	//echo $laaid;
	$db->monta_combo( 'laaid', $sql, 'S', '-- Informe o Andar --', '', '', '', '', '', '', '', $laaid );
	echo obrigatorio();
}

function montaUsuario($unaid){
	global $db;
	/*
    $sql = sprintf("SELECT
                     usucpf AS codigo,
                     usunome AS descricao
                    FROM
                     seguranca.usuario
                    WHERE
                     unaid = %d AND
                     (usustatus IS NULL OR
                     usustatus = 'A')
                    ORDER BY
                     usunome",
            $unaid);
    */
	$sql = "SELECT
			 u.usucpf AS codigo,
			 u.usunome AS descricao
			FROM
			 seguranca.usuario u
			 inner join seguranca.usuario_sistema us on
			 u.usucpf = us.usucpf
			 where
			 us.sisid = ".$_SESSION['sisid']." AND
			 us.susstatus = 'A' AND
			 us.suscod = 'A'
			ORDER BY
			 u.usunome;";
	$db->monta_combo( 'usucpfdemandante', $sql, 'S', '-- Informe o usuário --', '', '' );
	echo obrigatorio();
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

	if(!$_POST['dmdbacklog']) $_POST['dmdbacklog'] = 'f';
	if(!$_POST['dmdentrega']) $_POST['dmdentrega'] = 'f';

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
						dmdreproducao,
						dmdstatus,
						laaid,
						dmdsalaatendimento,
						unaid,
						dmdqtde,
						dmdhorarioatendimento,
                        celid,
						dmdatendremoto,
						dmddatainclusao,
						dmdatendurgente,
						dmdjusturgente,
						dmdjudicial,
						dmdbacklog,
						dmdentrega
					)VALUES(
						%s
						'%s',
						%s,
						%s,
						'%s',
						'%s',
						'%s',
						'A',
						 %s,
						'%s',
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
					) RETURNING dmdid ",
		(count($dado) > 0 ? implode(',',$select)."," : ''),
		(count($dado) > 0 ? "'".implode("','",$dado)."'," : ''),
		$_SESSION['usucpf'],
		$_POST['tipid'] ? $_POST['tipid'] : 'null',
		($_POST['sidid'] ? $_POST['sidid'] : 'null'),
		($_POST['assunto'] ? simec_addslashes($_POST['assunto']) : ''),
		($_POST['necessidade'] ? simec_addslashes($_POST['necessidade']) : ''),
		($_POST['reproducao'] ? simec_addslashes($_POST['reproducao']) : ''),
		($_POST['laaid'] ? $_POST['laaid'] : 'null'),
		$_POST['dmdsalaatendimento'],
		$_POST['unaid'],
		$_POST['dmdqtde'],
		$horario,
		($_POST['celid'] ? $_POST['celid'] : 'null'),
		$_POST['atendimentoRemoto'],
		date('Y-m-d H:i:s'),
		$_POST['dmdatendurgente'],
		$_POST['dmdjusturgente'],
		$_POST['dmdjudicial'],
		$_POST['dmdbacklog'],
		$_POST['dmdentrega']
	);
	//dbg($sql,1);

	$dmdid = $db->pegaUm($sql);

	if(!$dmdid){
		die('<script>
	 			alert("Problemas ao cadastrar a demanda. Favor avisar ao Gestor do sistema.");
	 			history.go(-1);
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
			$dmdid,
			$_POST['nserie'],
			$_POST['npatrimonio'],
			'A'
		);
		$db->executar($sql);
	}



	//Atualiza dados cadastrais do usuário
	if($usucpf){
		$sql = "select usdid from demandas.usuariodetalhes where usucpf = '$usucpf'";
		$usdid = $db->pegaUm($sql);

		$usdcelular = str_replace("-","",$_POST['usdcelular']);
		if(!$_POST['usdgabinete']) $_POST['usdgabinete'] = 'f';

		if(!$usdid){
			$sql = sprintf("INSERT INTO demandas.usuariodetalhes
					(
						usucpf,
						usdgabinete,
						usdramal,
						usdcelular,
						unaid,
						laaid,
						usdsala,
						usdnomecomputador
					)VALUES(
						'%s',
						'%s',
						'%s',
						'%s',
						%s,
						%s,
						'%s',
						'%s'
					)",
				$usucpf,
				$_POST['usdgabinete'],
				$_POST['usdramal'],
				$usdcelular,
				$_POST['unaid'],
				($_POST['laaid'] ? $_POST['laaid'] : 'null'),
				$_POST['dmdsalaatendimento'],
				$_POST['usdnomecomputador']
			);
		}
		else{
			$sql = " UPDATE demandas.usuariodetalhes SET
						usdgabinete = '".$_POST['usdgabinete']."',
						usdramal = '".$_POST['usdramal']."',
						usdcelular = '".$usdcelular."',
						unaid = ".$_POST['unaid'].",
						laaid = ".$_POST['laaid'].",
						usdsala = '".$_POST['dmdsalaatendimento']."',
						usdnomecomputador = '".$_POST['usdnomecomputador']."'
					 WHERE usdid = {$usdid}
					 and usucpf = '$usucpf'";

		}
		$db->executar($sql);


	}

	$db->commit();

	return $dmdid;
}

######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$dmdid){
	global $db;

	$dados = is_array($dados) ? $dados : array();
	
	if (!$arquivo || !$dmdid)
		return false;

	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'".$dados["arqdescricao"]."',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('d/m/Y')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);

	//Insere o registro na tabela demandas.anexos
	$sql = "INSERT INTO demandas.anexos
			(
				dmdid,
				arqid,
				anxdtinclusao,
				anxstatus
			)VALUES(
			    ". $dmdid .",
				". $arqid .",
				now(),
				'A'
			);";
	$db->executar($sql);

	if(!is_dir('../../arquivos/demandas/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/demandas/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
				$porcentagem = (100 * 640) / $original_x;
			}else {
				$porcentagem = (100 * 480) / $original_y;
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}


	$db->commit();
	return true;

}

######## Fim funções de UPLOAD ########

/*
 * Executa a função que filtra e retorna o <select>
 */
if ($_REQUEST['ordidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaTipoDemanda($_POST['ordidAjax']);
	//montaCelula($_POST['ordidAjax']);
	exit;
}
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

/*elseif ($_REQUEST['unaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaUsuario($_POST['unaidAjax']);
	exit;
}*/


//filtra tipo para mostrar qtd
if ($_REQUEST['tipidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaQtdDemanda($_POST['tipidAjax']);
	exit;
}


//filtra o andar
if ($_REQUEST['lcaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaAndarDemanda($_POST['lcaidAjax']);
	exit;
}


//filtra o sistema
if ($_REQUEST['celidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSistema($_POST['celidAjax']);
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

/*
 * Executa a função que salva a demanda
 * Retorna mensagem de sucesso e redireciona a página
 */
if($_POST['varaux'] == 'okCad') {

	$dmdid 	= salvaDemanda();

	if(!$dmdid){
		die('<script>
	 			alert("Problemas ao cadastrar a demanda. Favor avisar ao Gestor do sistema.");
	 			history.go(-1);
	 		</script>');
	}

	//envia email p/ banco de dados
	//if($_POST["ordid"] == "8"){
	//envia email pro solicitante
	enviaEmailCadDemanda($dmdid);
	//}

	$perfil	= arrayPerfil();

	if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], '', $dmdid)) {
		if((in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ||
				in_array(DEMANDA_PERFIL_ANALISTA_FNDE, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) ||
				in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfil)) && ($_POST["ordid"] == "1" || $_POST["ordid"] == "18" || $_POST["ordid"] == "19" || $_POST["ordid"] == "20" || $_POST["ordid"] == "21" || $_POST["ordid"] == "23" || $_POST["ordid"] == "24") ) {
			$_SESSION['dmdid'] = $dmdid;

			?>
			<script>
				alert("Sua Demanda de nº <?=$dmdid?> foi enviada com sucesso! \nAcompanhe o andamento da mesma na tela 'Listar Demandas'");

				var resposta = confirm('Deseja alocar recursos agora?');
				if(resposta)
					location.href='demandas.php?modulo=principal/atendimento&acao=A';
				else
					location.href='demandas.php?modulo=principal/cadDemanda&acao=A';
			</script>
		<?
		}
		else {
			?>
			<script>
				alert("Sua Demanda de nº <?=$dmdid?> foi enviada com sucesso! \nAcompanhe o andamento da mesma na tela 'Listar Demandas'");
				location.href='demandas.php?modulo=principal/cadDemanda&acao=A';
			</script>
		<?
		}
		die;
	} else {
		die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");
	}
}

function verificaDataDemanda ()
{
	global $db;
	$sql = "select
                    sisid, sisdsc, sisdtalteradados
                    , sisdtalteradados
                    --+ '60 days'::interval as sessentadias
                from seguranca.sistema
                    where sisid =44";

	$extrair = $db->pegaLinha($sql);


//        $sqlupdate = "update  seguranca.sistema
//                      SET sisdtalteradados = current_date
//                      WHERE sisid=44
//                      AND sisdtalteradados + '60 days'::interval > current_date";

	// $db->executar( $sqlupdate );

	return $extrair['sisdtalteradados'];
}

function calculaData($data,$operacao, $ano, $meses, $dias) {

	//informe a data no formato yyyy/mm/dd

	$data = explode("/", $data);

	if ($operacao == '+') {

		$novaData = date("y-m-d", mktime(0, 0, 0, $data[1] + $ano, $data[0] + $meses, $data[2] + $dias) ); //'2012-06-25'

	} else {

		$novaData = date("y-m-d", mktime(0, 0, 0, $data[1] - $ano,$data[0] - $meses, $data[2] - $dias) );

	}

	return $novaData;

}


function atualizaXmlDemanda () {

	$atualizaXml =  calculaData("2012-06-25","+",10,0,1);
	return $atualizaXml;

	//die(verificaDataDemanda());
	//die($resposta[0]);



}
//ver (atualizaXmlDemanda());

$usucpf = $_REQUEST['usucpf'] ? $_REQUEST['usucpf'] : $_SESSION['usucpf'];



//recupera dados



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
		p.priid,
		d.dmdsalaatendimento,
		d.laaid,
		p.pridsc,
		l.lcaid,
		d.unaid,
		d.dmdatendurgente,
		d.dmdjusturgente,
		u.usdramal,
		u.usdcelular,
		u.usdgabinete,
		u.usdsala,

		u.laaid as laaid2,
		l2.lcaid as lcaid2,
		--u.unaid as unaid2,
		u.usdramal as usdramal,
		d.dmdsalaatendimento as dmdsalaatendimento,
		--t.ordid  as tipid,
		(dmddatainclusao) as periodo,
		dmddatainclusao,
		(CURRENT_DATE) as datadehoje,
                ('".verificaDataDemanda()."') as hoje,



		   CASE WHEN (CURRENT_DATE >= '".verificaDataDemanda()."'::date) AND (dmddatainclusao::date < '".verificaDataDemanda()."'::date)  and o.ordid = 3 then NULL  else u.unaid  end as unaid2,
		   CASE WHEN (CURRENT_DATE >= '".verificaDataDemanda()."'::date) AND (dmddatainclusao::date < '".verificaDataDemanda()."'::date) and o.ordid = 3 then NULL  else l.lcaid  end as lcaid2,
		   CASE WHEN (CURRENT_DATE >= '".verificaDataDemanda()."'::date) AND (dmddatainclusao::date < '".verificaDataDemanda()."'::date) and o.ordid = 3 then NULL  else u.usdsala  end as usdsala,
		   CASE WHEN (CURRENT_DATE >= '".verificaDataDemanda()."'::date) AND (dmddatainclusao::date < '".verificaDataDemanda()."'::date) and o.ordid = 3 then NULL  else u.usdramal  end as usdramal,
		   CASE WHEN (CURRENT_DATE >= '".verificaDataDemanda()."'::date) AND (dmddatainclusao::date < '".verificaDataDemanda()."'::date) and o.ordid = 3 then NULL  else u.usdcelular  end as usdcelular,



		(CASE WHEN o.ordid = 2 OR o.ordid = 13 OR o.ordid = 14 OR o.ordid = 23 OR o.ordid = 24 THEN -- 2=Redes - 13=Gestão Documentos CGI
			t.celid
		  ELSE
			c.celid
		END) AS celid

		FROM demandas.demanda AS d

	LEFT JOIN demandas.prioridade AS p ON p.priid = d.priid
	LEFT JOIN demandas.tiposervico AS t ON t.tipid = d.tipid
	LEFT JOIN demandas.origemdemanda AS o ON o.ordid = t.ordid
	LEFT JOIN demandas.sistemadetalhe AS s ON s.sidid = d.sidid
	LEFT JOIN demandas.sistemacelula AS c ON c.sidid = d.sidid
	LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
	LEFT JOIN demandas.usuariodetalhes AS u ON u.usucpf = d.usucpfdemandante
	LEFT JOIN demandas.localandaratendimento AS l2 ON l2.laaid = u.laaid
	WHERE d.usucpfdemandante = '".$usucpf."' order by d.dmdid desc limit 1";







$dadosx = $db->carregar($sqlx);














($dadosx[0]['laaid']) ? $display = '' : $display = 'none';

$usdgabinete =  $dadosx[0]['usdgabinete'];
$usdramal =  $dadosx[0]['usdramal'];
$usdcelular =  formata_fone_fax($dadosx[0]['usdcelular']);


//setor


if($dadosx[0]['unaid2']){
	$unaid =  $dadosx[0]['unaid2'];
}
// 	else{
// 		$unaid =  $dadosx[0]['unaid'];
// 	}
//local

if($dadosx[0]['lcaid2']){
	$lcaid =  $dadosx[0]['lcaid2'];
}
//  	else{
//  		$lcaid =  $dadosx[0]['lcaid'];
//  	}
//andar
if($dadosx[0]['laaid2']){
	$laaid =  $dadosx[0]['laaid2'];
}
//  	else{
//  		$laaid =  $dadosx[0]['laaid'];
//  	}
//sala
if($dadosx[0]['usdsala']){
	$dmdsalaatendimento =  $dadosx[0]['usdsala'];
}
//else{
//$dmdsalaatendimento =  $dadosx[0]['dmdsalaatendimento'];
//}


$ordidx = $dadosx[0]['ordid'];
$celidx = $dadosx[0]['celid'];
$sididx = $dadosx[0]['sidid'];

$displaySis = 'none';
$displayCel = 'none';
if($ordidx == 1 || $ordidx == 18 || $ordidx == 19 || $ordidx == 20 || $ordidx == 21){
	$displaySis = '';
	$displayCel = '';
}else if($ordidx == 2 || $ordidx == 13 || $ordidx == 14 || $ordidx == 23 || $ordidx == 24){
	$displaySis = 'none';
	$displayCel = '';
}else if($ordidx == 11){
	$displaySis = '';
	$displayCel = 'none';
}

//($dadosx[0]['tipquantidade'] == 't')? $displayQtd = '' : $displayQtd = 'none';
if(in_array(DEMANDA_PERFIL_ANALISTA_FNDE, arrayPerfil())) {
	$ordidx = 1;
	$celidx = 20;
	$sididx = 288;
	$displaySis = '';
	$displayCel = '';
}

//fim recupera dados



//esconde o campo origem da demanda
if (count(arrayPerfil()) == 1 && in_array(DEMANDA_PERFIL_DEMANDANTE, arrayPerfil())) {
	$econde_campo = "style='display:none;'";
	$displaySis = 'none';
	$displayCel = 'none';
}

/*
$mostra_campo_fnde = "none";
if (in_array(DEMANDA_PERFIL_ANALISTA_FNDE, arrayPerfil())){
	$mostra_campo_fnde = "";
}
*/



include APPRAIZ ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de Demanda', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

?>

<body>
<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="formDemanda" action="" method="post" enctype="multipart/form-data" onsubmit="return validaForm();">
	<input type="hidden" name="usucpf" id="usucpf" value="<?=$usucpf;?>">
	<input type="hidden" name="outrouser" id="outrouser" value="0">
	<input type="hidden" name="usermail" id="usermail" value="">
	<input type="hidden" name="verificaqtd" id="verificaqtd" value="">
	<input type="hidden" name="varaux" id="varaux" value="">


	<table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr id="tr_user2">
			<td class="SubTituloDireita" style="width:39.5%;">Demandante:</td>
			<?
			$usudemandante = nomeUser($usucpf);
			?>
			<td><?= campo_texto( 'usudemandante', 'S', 'N', '', 40, 250, '', ''); ?>&nbsp;&nbsp;<a href="javascript:void(0);" onclick="popupPesqUser()" title="Cadastrar demanda em nome de outro usuário">Outro</a></td>
		</tr>
		<tr <?=$econde_campo?>>
			<td align='right' width="40%" class="SubTituloDireita">Qual é a origem da <b>demanda</B>?:</td>
			<td>
				<table width="100%">
					<?
					if($ordidx){
						$sql = "SELECT
							case when ordid = ".$ordidx." then
									'<input type=\'radio\' checked name=\'ordid\' value=\'' || ordid || '\' onclick=\'filtraTipo('|| ordid ||');\'/>' || orddescricao || '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
								else
									'<input type=\'radio\' name=\'ordid\' value=\'' || ordid || '\' onclick=\'filtraTipo('|| ordid ||');\'/>' || orddescricao || '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
							end AS campo

						   FROM
						    demandas.origemdemanda
						   WHERE
						    ordstatus = 'A'
							ORDER BY
							 orddescricao";

					}
					else{
						$sql = "SELECT
							'<input type=\'radio\' name=\'ordid\' value=\'' || ordid || '\' onclick=\'filtraTipo('|| ordid ||');\'/>' || orddescricao || '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'AS campo
						   FROM
						    demandas.origemdemanda
						   WHERE
						    ordstatus = 'A'
							ORDER BY
							 orddescricao";
					}
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
		<tr id="tr_celula" style="display:<?=$displayCel?>;">
			<td align='right' class="SubTituloDireita">A demanda é relacionada a qual <b>Célula</b>:</td>
			<td id="td_celula">
				<?php
				if($ordidx == 1 || $ordidx == 18 || $ordidx == 19 || $ordidx == 20 || $ordidx == 21){


					//filtra por celula do demandasnte avançado
					if ( in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, arrayPerfil()) && count(arrayPerfil())==1){
						$sql = "select
										ur.celid
										from demandas.usuarioresponsabilidade ur
										where rpustatus = 'A'
								and usucpf = '".$_SESSION['usucpf']."'
								and ur.celid is not null
								and pflcod not in (235) --(235 = perfil demandante)
								";
						$dadosCel = $db->carregarColuna($sql);

						if($dadosCel) $andCel = " AND ur.celid in (".implode(",", $dadosCel).")";
					}


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
							 --c.ordid = ".$ordidx." AND
							 c.ordid = 1 AND
							 c.celstatus = 'A'
							 $andCel
							ORDER BY
							 c.celnome;";
					//dbg($sql);
					$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --', 'filtraSistema', '','','','','celid','',$celidx);
				}
				else if($ordidx == 2 || $ordidx == 13 || $ordidx == 14 || $ordidx == 23 || $ordidx == 24){
					$sql = "SELECT
							 c.celid AS codigo,
							 c.celnome as DESCRICAO
							FROM
							 demandas.celula c
							WHERE
							 c.ordid = ".$ordidx." AND
							 c.celstatus = 'A'
							ORDER BY
							 c.celnome;";
					$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --', 'filtraTipoRede', '','','','','celid','',$celidx);
				}
				?>
			</td>
		</tr>
		<tr id="tr_sistema" style="display:<?=$displaySis?>;">
			<td  align='right' class="SubTituloDireita">A demanda é relacionada a qual <b>Sistema</b>:</td>
			<td id="listasistema">
				<?php
				if($celidx){
					$sql = sprintf("select
						s.sidid  AS codigo,
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao
					from
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid
					where  s.sidstatus = 'A'
					AND celid = %d
					order by 2",
						$celidx);
					//$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o Sistema --', '', '' );
					$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o sistema --', '', '','','','','','',$sididx);
					echo obrigatorio();
				}
				else{
					$sql = "SELECT
					 sidid AS codigo,
					 upper(sidabrev) ||' - '|| siddescricao AS descricao
					FROM
					 demandas.sistemadetalhe
					WHERE
					 sidstatus = 'X'
					ORDER BY
					 2";
					$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o sistema --','', '' );
					echo obrigatorio();

				}
				?>
			</td>
		</tr>
		<tr id="tr_tipo" <?=$econde_campo?> <?if(!$ordidx) echo "style='display:none'";?>>
			<td align='right' class="SubTituloDireita">Informe o <b>tipo da demanda</B>:</td>
			<td id='tipodemanda'>
				<?php
				if($celidx && $ordidx != 1 && $ordidx != 18 && $ordidx != 19 && $ordidx != 20 && $ordidx != 21) $andCelid = "and celid = ".$celidx;
				if($ordidx){
					$sql = sprintf("SELECT
					 tipid AS codigo,
					 tipnome AS descricao
					FROM
					 demandas.tiposervico
					WHERE
					 ordid = %d AND
					 tipstatus = 'A'
					 $andCelid
					ORDER BY
					 tipnome",
						$ordidx);
					$db->monta_combo( 'tipid', $sql, 'S', '-- Informe o tipo da demanda --', 'filtraQtd', '' );
					echo obrigatorio();
				}
				?>
			</td>
		</tr>
		<tr id="linhaDescTipoDemanda" style="display: none;">
			<td  class="SubTituloDireita">&nbsp;</td>
			<td id="descTipoDemanda"></td>
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
			<td align='right' class="SubTituloDireita">Descreva os passos para a reprodução do problema:<br><b>(Opcional)</b></td>
			<td><?= campo_textarea('reproducao', 'N ', 'S', '', 80, 5, 4000); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Anexe um arquivo:<br><b>(Opcional)</b></td>
			<td>
				<input name="anexo" type="file" style="text-align: left; width: 83ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" class="normal" size="81">
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Horário alternativo do atendimento:<br><b>(Opcional)</b></td>
			<td>
				<input type="checkbox" name="horarioA" value="A"> Horário de Almoço (12:00 às 14:00)
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox" name="horarioN" value="N"> Horário Noturno (18:00 às 22:00)
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Deseja receber atendimento remoto:</td>
			<td>
				<input type="radio" name="atendimentoRemoto" value="t" onclick="atendRemoto();"> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="atendimentoRemoto" value="f" onclick="atendRemoto();" checked> Não

			</td>
		</tr>
		<tr id="tr_atendremoto" style="display:none;">
			<td  align='right' class="SubTituloDireita">Nome do Computador(CPU):<br>Clique com o botão direito do mouse no atalho do "Meu computador", depois clique em "Propriedades", depois clique na aba "Nome do computador". Ex.: mec-32-2014.mec.gov.br (Digite somente mec-32-2014).</td>
			<td><?= campo_texto( 'usdnomecomputador', 'S', 'S', '', 40, 100, '', ''); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Atendimento é urgente:</td>
			<td>
				<input type="radio" name="dmdatendurgente" value="t" onclick="atendUrgente();"> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="dmdatendurgente" value="f" onclick="atendUrgente();" checked> Não
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Demanda Judicial:</td>
			<td>
				<input type="radio" name="dmdjudicial" value="t"> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="dmdjudicial" value="f" checked> Não
			</td>
		</tr>
		<tr id="tr_atendurgente" style="display:none;">
			<td  align='right' class="SubTituloDireita">Informe a justificativa:</td>
			<td><?= campo_textarea('dmdjusturgente', 'S ', 'S', '', 80, 5, 4000); ?></td>
		</tr>
		<tr>
			<td align='right' bgcolor="#DCDCDC"  >
				informe abaixo <b>o local de atendimento</b>:
			</td>
			<td bgcolor="#DCDCDC">&nbsp;</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Setor:</td>
			<td>
				<?php
				$sql = " SELECT
						  unaid AS codigo,
					  	  upper(unasigla)||' - '||unadescricao as descricao
						 FROM
						  demandas.unidadeatendimento
						 WHERE
						  unastatus = 'A'
						  order by unasigla, unadescricao";
				$db->monta_combo("unaid",$sql,'S',"-- Selecione um setor --",'','', '', '', '', 'unaid', '');
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Edifício:</td>
			<td>
				<?php
				$sql = "SELECT
				 lcaid AS codigo,
				 lcadescricao AS descricao
				FROM
				 demandas.localatendimento
				ORDER BY
				 lcadescricao";
				$db->monta_combo( 'lcaid', $sql, 'S', '-- Informe o local de atendimento --', 'filtraAndar', '', '', '', '', 'lcaid', '' );
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr id="tr_andar" style="display:<?=$display?>;">
			<td align='right' class="SubTituloDireita" >Andar:</td>
			<td id='andardemanda'>
				<?
				if($lcaid){
					$sql = sprintf("SELECT
								 laa.laaid AS codigo,
								 aa.anddescricao AS descricao
								 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao
								FROM
								 	demandas.localandaratendimento laa
									inner join demandas.andaratendimento aa on laa.andid = aa.andid
								WHERE
								 laa.lcaid = %d
								ORDER BY
								 aa.anddescricao",
						$lcaid);
					$db->monta_combo( 'laaid', $sql, 'S', '-- Informe o Andar --', '', '', '', '', '', 'laaid', '', $laaid );
				}
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<tr id="tr_sala" style="display: <?=$display?>;">
			<td align='right' class="SubTituloDireita">Sala:</td>
			<td><?= campo_texto( 'dmdsalaatendimento', 'S', 'S', '', 20, 50, '', '', '', '', '', 'id="dmdsalaatendimento"'); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Sala fica no Gabinete:</td>
			<td>
				<input type="radio" name="usdgabinete" id="usdgabinete" value="t" <?if($usdgabinete == 't') echo "checked";?>> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="usdgabinete" id="usdgabinete" value="f" <?if($usdgabinete == 'f' || $usdgabinete == '') echo "checked";?>> Não
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Ramal:</td>
			<td><?= campo_texto( 'usdramal', 'S', 'S', '', 4, 4, '####', '', '', '', '', 'id="usdramal"'); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Celular:<br><b>(Opcional)</b></td>
			<td><?= campo_texto( 'usdcelular', 'N', 'S', '', 10, 9, '####-####', '', '', '', '', 'id="usdcelular"'); ?></td>
		</tr>
		<tr id="tr_backlog" style="display: none;">
			<td align='right' class="SubTituloDireita">BackLog:</td>
			<td>
				<input type="radio" name="dmdbacklog" value="t"> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="dmdbacklog" value="f" checked> Não
			</td>
		</tr>
		<tr id="tr_entrega" style="display: none;">
			<td align='right' class="SubTituloDireita">Entrega:</td>
			<td>
				<input type="radio" name="dmdentrega" value="t"> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="dmdentrega" value="f" checked> Não
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td width="15%">&nbsp;</td>
			<td>
				<input type="submit" class="botao" name="btalterar" value="Enviar">
			</td>
		</tr>
		<tr>
			<td>
				<br>
				<b>Demandas em Aberto</b>
				<br>
				<br>
			</td>
		</tr>
	</table>
</form>



<?php

$sql = "SELECT
				d.dmdid AS codigo,
				d.dmdtitulo,
				CASE
				  WHEN d.docid IS NOT NULL THEN esddsc
				  ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>Em Processamento</span>'
				END AS situacao,
				to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') AS dmddatainclusao,
				to_char(d.dmddataconclusao, 'DD-MM-YYYY HH24:MI:SS') AS dmddataconclusao,
				CASE
				  WHEN u.usucpf IS NOT NULL THEN usunome
				  ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>Em Processamento	</span>'
				END AS responsavel
			FROM
				demandas.demanda AS d
				LEFT JOIN workflow.documento doc ON doc.docid 	  = d.docid
				LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
				LEFT JOIN seguranca.usuario u ON u.usucpf 		  = d.usucpfexecutor
			WHERE
				d.dmdstatus = 'A'
				AND d.usucpfdemandante   = '".$usucpf."'
				AND (doc.esdid in (91,107,92,108) OR doc.esdid is null)
			ORDER BY
				 d.dmddatainclusao DESC";

$cabecalho = array( "Cód" , "Título da Demanda", "Situação", "Data de Abertura", "Data de Encerramento","Solucionado por");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');

$flag_duplicado = $db->PegaUm( $sql );
?>



<script type="text/javascript">
	d = document;

	function atendRemoto(){

		tr  = d.getElementById('tr_atendremoto');

		if(d.getElementsByName('atendimentoRemoto')[0].checked == true){
			tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}
		if(d.getElementsByName('atendimentoRemoto')[1].checked == true){
			tr.style.display = 'none';
		}
	}


	function atendUrgente(){

		tr  = d.getElementById('tr_atendurgente');

		if(d.getElementsByName('dmdatendurgente')[0].checked == true){
			tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}
		if(d.getElementsByName('dmdatendurgente')[1].checked == true){
			tr.style.display = 'none';
		}
	}

	/*
	 function cargoDas(){

	 tr  = d.getElementById('tr_cargodas');

	 if(d.getElementsByName('dmdcargodas')[0].checked == true){
	 tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	 }
	 if(d.getElementsByName('dmdcargodas')[1].checked == true){
	 tr.style.display = 'none';
	 }
	 }
	 */

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


	function outroUsuario(ger){
		tr  = d.getElementById('tr_unigest');
		tr1 = d.getElementById('tr_usercad');
		tr2 = d.getElementById('tr_user');
		tr3 = d.getElementById('tr_user2');

		selectUnigest = d.getElementsByName('unaid')[0];
		selectUnigest.options[0].selected = 'true';

		if (ger == 1){
			tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
//		tr1.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
//		tr2.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
//		tr3.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}else{
			tr.style.display  = 'none';
			tr1.style.display = 'none';
			tr2.style.display = 'none';
			tr3.style.display = 'none';
		}


	}

	function userCadastrado(ger){
		tr2 = d.getElementById('tr_user');
		tr3 = d.getElementById('tr_user2');

		if (ger == 1){
			tr3.style.display = 'none';
			tr2.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}else{
			tr2.style.display = 'none';
			tr3.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';

		}
	}

	/*
	 * Faz requisição via ajax
	 * Filtra o tipo de damanda, atravéz do parametro passado 'ordid'
	 */
	function filtraTipo(ordid) {

		$('linhaDescTipoDemanda').hide();

		if( $('container-desc') )
		{
			$('container-desc').replace('');
		}


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

		if(ordid == 1 || ordid == 18 || ordid == 19 || ordid == 20 || ordid == 21){ //Sistemas de Informação
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
		var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
			method:     'post',
			parameters: '&ordidAjax=' + ordid,
			onComplete: function (res)
			{
				td.innerHTML = res.responseText;

				var input       = $$('select[name="tipid"]')[0];
				var origemSelecionada;
				var descricao   = '';
				var idSelected  = false;
				var options;
				var origemDisponiveis = ['18','19','20','21'];

				$(input).observe('change',function(){

					options             = $(this).getElementsBySelector('option');
					origemSelecionada   = $$('input[name="ordid"]:checked')[0].value;

					options.each(function(option){
						if(option.selected == true){
							idSelected = option.value;
						}
					});

					if( idSelected == false) {
						$('linhaDescTipoDemanda').hide();
						return false;
					}

					if( origemDisponiveis.indexOf( origemSelecionada ) >= 0){

						options.each(function(option){
							if(option.selected == true){
								idSelected = option.value;
							}
						});

						var descricao = $('tipoServico'+idSelected).value;
						$('descTipoDemanda').innerHTML =  descricao;
						$('linhaDescTipoDemanda').show();
					}
				});


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
				var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
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
			var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
				method:     'post',
				parameters: '&tipidAjax=' + tipid,
				onComplete: function (res)
				{

					var result = res.responseText.replace(/\s+/g,"");

					if(result != "N"){
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
		var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
			method:     'post',
			parameters: '&celidAjax=' + celid,
			onComplete: function (res)
			{
				td.innerHTML = res.responseText;
				tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';

				if(celid == 2){
					d.getElementById('tr_backlog').style.display = '';
					d.getElementById('tr_entrega').style.display = '';
				}
				else{
					d.getElementById('tr_backlog').style.display = 'none';
					d.getElementById('tr_entrega').style.display = 'none';
				}
			}
		});

	}



	/*
	 * Faz requisição via ajax
	 * Filtra o andar, atravéz do parametro passado 'lcaid'
	 */
	function filtraAndar(lcaid) {

		if(!lcaid) lcaid = '999999';

		trs	   = d.getElementById('tr_sala');
		tr 	   = d.getElementById('tr_andar');
		td     = d.getElementById('andardemanda');
		select = td.getElementsByTagName('select')[0];
		//trSis  = d.getElementById('tr_sistema');

		//trSis.style.display = paramSystem == 1 ? (navigator.appName == 'Netscape' ? 'table-row' : 'block') : 'none';

		// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
		if (select)
			select.disabled = true;

		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
			method:     'post',
			parameters: '&lcaidAjax=' + lcaid,
			onComplete: function (res)
			{
				td.innerHTML = res.responseText;
				tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
				trs.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
			}
		});
	}



	/*
	 * Faz requisição via ajax
	 * Filtra o usuário, atravéz do parametro passado 'unaid'
	 */
	function filtraUser(unaid) {

		tr     = d.getElementById('tr_unigest');
		tr1    = d.getElementById('tr_usercad');
		tr2    = d.getElementById('tr_user');
		tr3    = d.getElementById('tr_user2');
		td     = d.getElementById('td_user');
		select = td.getElementsByTagName('select')[0];

		if (!unaid){
			tr1.style.display = 'none';
			tr2.style.display = 'none';
			tr3.style.display = 'none';
			return;
		}

		// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
		if (select){
			select.options[0].text = 'Aguarde...';
			select.disabled = true;
		}
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
			method:     'post',
			parameters: '&unaidAjax=' + unaid,
			onComplete: function (res)
			{
				tr1.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
				td.innerHTML = res.responseText;
			}
		});
	}




	/*
	 * Function destinada a validação do formulário
	 */
	function validaForm()
	{
		elementos = d.getElementsByName('ordid');
		var checado = '';
		var flag = "<?=$econde_campo?>";


		if(!flag){

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


			/*
			 if (d.getElementById('outrouser1').checked){
			 if (d.getElementsByName('unaid')[0].value == ''){
			 alert('O Campo Unidade Gestora é obrigatório!');
			 d.getElementsByName('unaid')[0].focus();
			 return false;
			 }

			 if (!d.getElementsByName('usucad')[0].checked && !d.getElementsByName('usucad')[1].checked){
			 alert('O campo usuário cadastrado é obrigatório!');
			 return false;
			 }

			 if (d.getElementsByName('usucad')[0].checked && d.getElementsByName('usucpfdemandante')[0].value == ''){
			 alert('A combo do nome do usuário é obrigatório!');
			 return false;
			 }

			 if (d.getElementsByName('usucad')[1].checked && d.getElementsByName('usernome')[0].value == ''){
			 alert('O campo nome do usuário é obrigatório!');
			 d.getElementsByName('usernome')[0].focus();
			 d.getElementsByName('usernome')[0].select();
			 return false;
			 }

			 }
			 */


			if (checado != 1){
				alert('O campo origem da demanda, deve ser preenchido!');
				return false;
			}
			if (d.getElementsByName('tipid')[0].value == ''){
				alert('O campo tipo da demanda, deve ser preenchido!');
				d.getElementsByName('tipid')[0].focus();
				return false;
			}
		}




		if(d.getElementById('verificaqtd').value != ""){
			if (d.getElementById('dmdqtde').value < 1){
				alert('O campo quantidade do serviço, deve ser maior que zero!');
				d.getElementById('dmdqtde').focus();
				return false;
			}
		}


		if(!flag){
			//683 = suporte de atendimento - "29 - Chamados para garantia (fornecedores) - REMOTO"
			if (d.getElementsByName('tipid')[0].value == '683'){
				if (d.getElementsByName('nserie')[0].value == '' && d.getElementsByName('npatrimonio')[0].value == ''){
					alert('O campo nº de série ou nº de patrimônio, deve ser preenchido!');
					d.getElementsByName('nserie')[0].focus();
					return false;
				}
			}
		}

		if (d.getElementsByName('assunto')[0].value == ''){
			d.getElementsByName('assunto')[0].focus();
			d.getElementsByName('assunto')[0].select();
			alert('O campo assunto, deve ser preenchido!');
			return false;
		}
		/*
		 if (d.getElementsByName('prioridade')[0].value == ''){
		 d.getElementsByName('prioridade')[0].focus();
		 alert('O campo prioridade, deve ser preenchido!');
		 return false;
		 }
		 */
		if (d.getElementsByName('necessidade')[0].value == ''){
			d.getElementsByName('necessidade')[0].focus();
			d.getElementsByName('necessidade')[0].select();
			alert('O campo necessidade, é obrigatório!');
			return false;
		}

		/*
		 if(d.getElementsByName('dmdcargodas')[0].checked == true){
		 if(d.getElementsByName('carid')[0].value == ''){
		 alert('O campo Cargo DAS, deve ser preenchido!');
		 d.getElementsByName('carid')[0].focus();
		 return false;
		 }
		 }
		 */

		if(d.getElementsByName('atendimentoRemoto')[0].checked == true){
			if(trim(d.getElementsByName('usdnomecomputador')[0].value) == ''){
				alert('O campo Nome do Computador(CPU), deve ser preenchido!');
				d.getElementsByName('usdnomecomputador')[0].focus();
				return false;
			}
		}

		if(d.getElementsByName('dmdatendurgente')[0].checked == true){
			if(trim(d.getElementsByName('dmdjusturgente')[0].value) == ''){
				alert('O campo Justificativa, deve ser preenchido!');
				d.getElementsByName('dmdjusturgente')[0].focus();
				return false;
			}
		}

		if(d.getElementsByName('unaid')[0].value == ''){
			alert('O campo Setor, deve ser preenchido!');
			d.getElementsByName('unaid')[0].focus();
			return false;
		}

		//if(elementos[0].checked == true || elementos[3].checked == true || elementos[4].checked == true){
		if(d.getElementsByName('lcaid')[0].value == ''){
			alert('O campo Edifício, deve ser preenchido!');
			d.getElementsByName('lcaid')[0].focus();
			return false;
		}
		if(d.getElementsByName('laaid')[0].value == ''){
			alert('O campo Andar, deve ser preenchido!');
			d.getElementsByName('laaid')[0].focus();
			return false;
		}
		if(d.getElementsByName('dmdsalaatendimento')[0].value == ''){
			alert('O campo Sala, deve ser preenchido!');
			d.getElementsByName('dmdsalaatendimento')[0].focus();
			return false;
		}
		if(d.getElementsByName('usdramal')[0].value == ''){
			alert('O campo Ramal, deve ser preenchido!');
			d.getElementsByName('usdramal')[0].focus();
			return false;
		}
		if(d.getElementsByName('usdramal')[0].value.length < 4){
			alert('Ramal Inválido, Favor informar o ramal correto!');
			d.getElementsByName('usdramal')[0].focus();
			return false;
		}
		if(d.getElementsByName('usdcelular')[0].value != "" && d.getElementsByName('usdcelular')[0].value.length < 9){
			alert('Celular Inválido, Favor informar o celular correto!');
			d.getElementsByName('usdcelular')[0].focus();
			return false;
		}

		//}


		<?if($flag_duplicado){?>
		if(confirm('Existe(m) Demanda(s) em Aberto. \nVerifique na lista abaixo se esta demanda já existe. \nDeseja cadastrar mesmo assim?'))
		{
			d.getElementsByName('varaux')[0].value = 'okCad';
			return true;
		}else{
			return false;
		}
		<?}else{?>
		d.getElementsByName('varaux')[0].value = 'okCad';
		return true;
		<?}?>


	}

	/*
	 var elementos = d.getElementsByName('ordid');
	 for (i=0; i < elementos.length; i++){
	 if (elementos[i].value == 1){//Sistemas de Informação
	 elementos[i].checked = true;
	 break;
	 }
	 }
	 filtraTipo(1);
	 var elementos = d.getElementsByName('celid');
	 for (i=0; i < elementos.length; i++){
	 if (elementos[i].value == 20){//Celula M - nieliton
	 elementos[i].checked = true;
	 break;
	 }
	 }
	 //d.getElementsByName('celid').selectedIndex = '20';
	 //d.getElementsByName('celid').options[selIndex].value = true;

	 //filtraSistema(20);
	 */

</script>
</body>
