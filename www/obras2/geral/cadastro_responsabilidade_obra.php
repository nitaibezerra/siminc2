<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

 /*
   Sistema Simec
   Setor responsável: --
   Desenvolvedor: Equipe Consultores Simec
   Analista: Mario Cesar
   Programador: Eduardo Dunice (e-mail: eduardo.neto@mec.gov.br)
   Módulo:seleciona_obra_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

include_once APPRAIZ . "www/obras2/_constantes.php"; 
include_once APPRAIZ . "www/obras2/_funcoes.php"; 
include_once APPRAIZ . "www/obras2/_componentes.php";

$db     	= new cls_banco();
$usucpf 	= $_REQUEST['usucpf'];
$pflcod 	= $_REQUEST['pflcod'];
$requisicao = $_REQUEST["requisicao"];
$orgid  	= $_REQUEST['orgid'];
$estuf  	= $_REQUEST['estuf'];
$unidade 	= $_REQUEST['unidade'];
$campus 	= $_REQUEST['campus'];
$obra    	= $_REQUEST["obra"];

if ( !$db->testa_superuser() ){
	$sql = "SELECT distinct
			    oo.orgid
			FROM
				obras2.orgao oo
			INNER JOIN obras2.usuarioresponsabilidade ur ON ur.orgid = oo.orgid
			WHERE
				orgstatus = 'A' AND				
				rpustatus = 'A' AND
				ur.estuf IS NULL AND
				ur.entid IS NULL AND
				ur.usucpf = '{$_SESSION["usucpf"]}'";

	$orgid = $db->pegaUm($sql);
	if (!$orgid){
		die('<script type="text/javascript">
				alert(\'Seu perfil não permite liberar acesso ao sistema!\');
				window.close();
			 </script>');	
	}
	
}

if( $pflcod == PFLCOD_SUPERVISOR_UNIDADE ){
	$sql = "SELECT DISTINCT
				ur.entid,
				e.orgid,
				ee.estuf
			FROM
				obras2.usuarioresponsabilidade ur
			INNER JOIN obras2.empreendimento e ON e.entidunidade = ur.entid AND 
												  e.empstatus = 'A'
			INNER JOIN entidade.endereco     ee ON ee.endid = e.endid
			WHERE
				ur.usucpf = '{$usucpf}' AND 
				ur.pflcod in (" . PFLCOD_SUPERVISOR_UNIDADE . ", " . PFLCOD_EMPRESA_VISTORIADORA_FISCAL . ", " . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . ") AND
				rpustatus = 'A'";

	$dados = $db->carregar($sql);
	
	if(is_array($dados)){
		$usuestufs1 = '';
		$usuestufs2 = '';
		$usuestufs3 = '';
		$usuestufs5 = '';
		foreach($dados as $dado){
			$usuentids .= $usuentids ? ",".$dado['entid'] : $dado['entid'];  
			$usuorgids .= $usuorgids ? ",".$dado['orgid'] : $dado['orgid']; 
			if( $dado['estuf'] != '' ){
				if( $dado['orgid'] == 1 ){
					$usuestufs1 .= ($usuestufs1 != '') ? ",'".$dado['estuf']."'" : "'".$dado['estuf']."'"; 
				} 
				if( $dado['orgid'] == 2 ){
					$usuestufs2 .= ($usuestufs2 != '') ? ",'".$dado['estuf']."'" : "'".$dado['estuf']."'"; 
				} 
				if( $dado['orgid'] == 3 ){
					$usuestufs3 .= ($usuestufs3 != '') ? ",'".$dado['estuf']."'" : "'".$dado['estuf']."'"; 
				} 
				if( $dado['orgid'] == 5 ){
					$usuestufs5 .= ($usuestufs5 != '') ? ",'".$dado['estuf']."'" : "'".$dado['estuf']."'"; 
				} 
			}
		}
	}else{
		die('<script type="text/javascript">
				alert(\'Para vincular uma obra, é necessário possuir uma entidade vinculada!\');
				window.close();
			 </script>');	
	}
	
// Perfil EMPRESA	
}elseif( $pflcod == PFLCOD_EMPRESA_VISTORIADORA_FISCAL || $pflcod == PFLCOD_EMPRESA_VISTORIADORA_GESTOR ){
	$sql = "SELECT DISTINCT
				ur.estuf
			FROM
				obras2.usuarioresponsabilidade ur
			WHERE
				ur.usucpf = '{$usucpf}' AND 
				ur.pflcod IN (" . PFLCOD_EMPRESA_VISTORIADORA_FISCAL . ", " . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . ") AND
				ur.estuf IS NOT NULL AND
				rpustatus = 'A'";

	$arEstuf = $db->carregarColuna($sql);
	
	if (!$arEstuf){
		die('<script type="text/javascript">
				alert(\'Para vincular uma obra, é necessário possuir um estado!\');
				window.close();
			 </script>');	
	}
}

function recuperaOrgao ($orgid = null){
	global $db;
	
	if ($db->testa_superuser()){

		$sql = "SELECT
					orgdesc 
				FROM
					obras2.orgao
				WHERE
					orgstatus = 'A' AND
					orgid = {$orgid}";
		
	}else{
		
		$sql = "SELECT
					orgdesc 
				FROM
					obras2.orgao oo
				INNER JOIN obras2.usuarioresponsabilidade ur ON oo.orgid = ur.orgid
				WHERE
					orgstatus = 'A' AND 
					rpustatus = 'A' AND
					usucpf = '{$_SESSION["usucpf"]}'";
		
	}
	
	
	return $dsc = $db->pegaUm($sql);
}

function selectUF(){
	global $db,$pflcod,$orgid,$estuf,$usuestufs1,$usuestufs2,$usuestufs3,$usuestufs5,$arEstuf;
	
	if( $pflcod == PFLCOD_SUPERVISOR_UNIDADE ){
		if ( $orgid == '1' ){
			$ufs = strtolower($usuestufs1);
		}  
		if ( $orgid == '2' ){
			$ufs = strtolower($usuestufs2);
		} 
		if ( $orgid == '3' ){
			$ufs = strtolower($usuestufs3);
		} 
		if ( $orgid == '5' ){
			$ufs = strtolower($usuestufs5);
		} 
		
		if ($ufs){
			$where = "WHERE
						lower(estuf) in ({$ufs})";
		}	
	}elseif($pflcod == PFLCOD_EMPRESA_VISTORIADORA_FISCAL || $pflcod == PFLCOD_EMPRESA_VISTORIADORA_GESTOR){
		$where = (count($arEstuf) ? "WHERE lower(estuf) IN ('" . strtolower(implode("', '", $arEstuf)) . "')" : "");
	}else{
		$where = "";
	}
	
	$sql = "SELECT
				estuf AS codigo,
				estuf || ' - ' || estdescricao AS descricao
			FROM
				territorios.estado
			{$where}
			ORDER BY
				estuf";
			
	return $db->monta_combo('estuf',$sql,'S',"-- Selecione para filtrar --",'limparCampos(1);filtroObras','','',170);
}

if( $_REQUEST['requisicao'] == 'uf' ){
	
	selectUF();
	die();
}

function selectUnidade(){
	global $db,$orgid,$estuf,$usuentids,$pflcod;
	
	if( $pflcod == PFLCOD_SUPERVISOR_UNIDADE ){
		$where = " AND entidunidade in ({$usuentids})";
	}else{
		$where = "";
	}
	
	$sql = "SELECT DISTINCT
				ee.entid as codigo,
				CASE WHEN ee.entnome is not null AND ee.entsig NOT LIKE '--'
					THEN upper(ee.entsig) || ' - ' || upper(ee.entnome) 
					ELSE upper(ee.entnome) 
				END as descricao
			FROM 
				obras2.empreendimento ep
			INNER JOIN entidade.entidade 		ee ON ee.entid	= ep.entidunidade
			INNER JOIN entidade.endereco 		ed ON ed.endid 	= ep.endid
			INNER JOIN territorios.municipio 	tm ON tm.muncod = ed.muncod
			--INNER JOIN obras2.situacaoobra 		so ON so.stoid 	= ep.stoid
			WHERE
				ep.empstatus = 'A' AND 
				ep.orgid = {$orgid} AND
				ed.estuf = '{$estuf}'
				{$where}
			GROUP BY
				ee.entnome,
				ee.entid,
				ee.entsig
			ORDER BY
				descricao";
				
	return $db->monta_combo('unidade',$sql,'S',"-- Selecione para filtrar --",'limparCampos(2);filtroObras','','',170);
}

if( $_REQUEST['requisicao'] == 'unidade' ){
	
	selectUnidade();
	die();
}

function selectCampus(){
	global $db,$orgid,$estuf,$unidade;
	
	if( $orgid == ORGID_EDUCACAO_BASICA ){
		$where = "";
	}else{
		$where = "";
	}
	
	$sql = "(SELECT 
				ee.entid as codigo,
				upper(ee.entnome) as descricao
			FROM 
				entidade.entidade ee
			INNER JOIN obras2.empreendimento ep ON ep.entidcampus = ee.entid
			INNER JOIN entidade.funcaoentidade ef on ee.entid = ef.entid
			INNER JOIN entidade.funentassoc ea on ea.fueid = ef.fueid
			INNER JOIN entidade.endereco ed ON ed.endid = ep.endid
			INNER JOIN territorios.municipio tm ON tm.muncod = ed.muncod
			--INNER JOIN obras2.situacaoobra so ON so.stoid = oi.stoid
			WHERE 
				ef.funid in (17,18,75) AND 
				ea.entid = {$unidade} AND
				ep.empstatus = 'A' 
			GROUP BY
				ee.entnome,
				ee.entid
			ORDER BY
				descricao)
				
			UNION ALL
			
			(SELECT
				'0' as codigo,
				'Sem campus' as descricao)";
	
	return $db->monta_combo('campus',$sql,'S',"-- Selecione para filtrar --",'limparCampos(3);filtroObras','','',170);
}

if( $_REQUEST['requisicao'] == 'campus' ){
	
	selectCampus();
	die();
}

function selectObras(){
	
	global $db,$orgid,$estuf,$unidade,$campus,$pflcod,$usucpf;
	
	$where = (($orgid != ORGID_EDUCACAO_BASICA) && ($orgid != ORGID_HOSPITAIS) && !empty($campus)) ? "AND ep.entidcampus = ".$campus : "";
	$where .= " AND ep.orgid = {$orgid} ";
	
	$sql = "SELECT DISTINCT
				ep.empid as codigo,
				CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) 
					THEN o.obrnome || ' - ' || m.mundescricao || ' - ' || ed.estuf
					ELSE ep.empdsc 
				END as descricao,
				CASE WHEN rpuid is not null
					THEN TRUE
					ELSE FALSE
				END as checked,
				o.obrid as obrid
			FROM 
				obras2.empreendimento ep
            INNER JOIN obras2.obras                     o ON o.empid = ep.empid AND o.obrstatus = 'A' AND o.obridpai is null
			left JOIN entidade.entidade 	 			ee ON ee.entid = ep.entidunidade
			LEFT  JOIN entidade.endereco 	 			ed ON ed.endid = ep.endid
			LEFT  JOIN territorios.municipio 			m ON m.muncod = ed.muncod
			LEFT  JOIN obras2.usuarioresponsabilidade 	ur ON ur.empid = ep.empid AND
														ur.usucpf = '{$usucpf}' AND 
														ur.pflcod = '{$pflcod}' AND
														ur.rpustatus = 'A'
			WHERE
				ep.empstatus = 'A' AND
				ee.entid = {$unidade} {$where}
			ORDER BY
				descricao";
//	ver($sql);
	$dados = $db->carregar($sql);
	
	if(is_array($dados)){
		
		$lista = "<table class=\"listagem\" cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"100%\">";
		
		$linhas = count($dados) - 1;
		
		$lista .= "<thead>
					<tr>
						<td class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\">
							<input id=\"checkPai\" onclick=\"marcatudo();\" type=\"checkbox\" \"> 
							<input type=\"hidden\">
						</td>
						<td class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\">
							<strong>ID</strong>
						</td>
						<td class=\"title\" bgcolor=\"\" align=\"left\" valign=\"top\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\">
							<strong>Descrição</strong>
						</td>
				  	</tr>
				   </thead>
				   <tbody>";	
		
		$orgao = recuperaOrgao($orgid);
		
		$cor = "";
		$i = 0;
		foreach($dados as $dado){
			
			if( $dado['checked'] == 't' ){
				$checked = "checked=\"true\"";
			}else{
				$checked = "";
			}
			
			$lista .= "<tr id=\"tr{$dado['codigo']}\" style=\"display:table-row;\" bgcolor=\"{$cor}\" onmouseout=\"this.bgColor='{$cor}';\" onmouseover=\"this.bgColor='#ffffcc';\">
						<td name=\"{$dado['codigo']}\"align=\"center\">
							<input type=\"Checkbox\" class=\"chkbox\" name=\"empid\"  value=\"".$orgid."|".$dado['codigo']."\" id=\"".$dado['codigo']."\" {$checked} onclick=\"retorna('".$i."');\">
							<input type=\"hidden\"   name=\"empdsc\" value=\"". $dado['codigo']."|".$orgao."|". $dado['descricao'] ."\">
						</td>
						<td align=\"center\">
							{$dado['obrid']}
						</td>
						<td align=\"left\">
							{$dado['descricao']}
						</td>
					  </tr>";	
			$cor = $cor == '#f7f7f7' ? '' : '#f7f7f7';
			$i++;
		}
		
		$lista .= "</tbody></table>";
	}
	return $lista;
}

if( $_REQUEST['requisicao'] == 'obras' ){
	
	echo selectObras();
	die();
}

function atribuiObras($usucpf, $pflcod, $empid){
	
	global $db;


	$data = date("Y-m-d H:i:s");
	
	$sql = "UPDATE obras2.usuarioresponsabilidade SET 
				rpustatus = 'I' 
		   	WHERE 
				usucpf = '{$usucpf}' AND 
				pflcod = '{$pflcod}' AND 
				empid IS NOT NULL";
	$sql_zera = $db->executar($sql);
	
	if (is_array($empid) && !empty($empid[0])){
		$count = count($empid);

        function pegaObraPorEmpid($empid)
        {
            global $db;

            $sql = "SELECT * FROM obras2.obras WHERE obridpai IS NULL AND obrstatus = 'A' AND empid = $empid";
            return $db->pegaLinha($sql);
        }

        function verificaObraPossuiFiscal($empid)
        {
            global $db;

            $sql = "
                SELECT
                    COUNT(*)
                FROM obras2.usuarioresponsabilidade ur
                inner join seguranca.usuario u on u.usucpf = ur.usucpf AND u.suscod = 'A'
                inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                WHERE ur.empid = $empid AND ur.rpustatus = 'A' AND ur.pflcod = 948;
            ";
            return ($db->pegaUm($sql) > 0) ? true : false;
        }

        $msg = "Operação realizada com sucesso!";

		// Insere a nova unidade
		$sql_insert = "INSERT INTO obras2.usuarioresponsabilidade (
							empid,
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
			
			list(,$empreendimento) = explode("|", $empid[$i]);

            // Verifica se a obra já possui um fiscal ativo.
            $fiscalCadastrada = verificaObraPossuiFiscal($empreendimento);

            if($fiscalCadastrada && $pflcod == PFLCOD_SUPERVISOR_UNIDADE){
                $obra = pegaObraPorEmpid($empreendimento);
                $msg .= "\\n\\n* A Obra ({$obra['obrid']}) {$obra['obrnome']} já possui um fiscal ativo. Só é possível vincular um único fiscal por obra.";
                continue;
            }


			if ( $empreendimento != $empreendimento_antiga ){
				$arrSql[] = "(
								'{$empreendimento}',
								'{$usucpf}', 
								'A', 
								'{$data}', 
								'{$pflcod}'
							 )";
			}
			
			$empreendimento_antiga = $empreendimento;
			
		}

        if(!empty($arrSql)) {
            $sql_insert = (string)$sql_insert . implode(",", $arrSql);
            $db->executar($sql_insert);
        }
	}
	$db->commit();
	echo "<script>
			alert(\"{$msg}\");
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>";
    exit;
	
}

if( $_REQUEST['gravar'] == 1 ){
	atribuiObras($usucpf, $pflcod, $_REQUEST['empresp']);
}

function buscaObrasCadastradas($usucpf, $pflcod){
	
	global $db, $obra;
	
	if (!$_POST['gravar'] && $_REQUEST["empresp"][0]){
		foreach ($_REQUEST["empresp"] as $v){
			list(,$emprendimento[]) = explode('|', $v );
		}
		$where = " ep.empid IN (".implode(',',$emprendimento).") ";
	}else{
		$where = " (ur.usucpf = '{$usucpf}' AND 
			 	    ur.pflcod = {$pflcod})  ";	
	}

	$sql = "SELECT DISTINCT 
				ep.empid as codigo, 
				CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
					ep.empdsc || ' - ' || m.mundescricao || ' - ' || ed.estuf 
				ELSE 
					ep.empdsc 
				END as descricao,
				ep.orgid,
				o.obrid as obrid
			FROM 
				obras2.empreendimento ep
		    INNER JOIN obras2.obras                     o ON o.empid = ep.empid AND o.obrstatus = 'A' AND o.obridpai is null
			INNER JOIN entidade.entidade 	 		   	 e ON e.entid = ep.entidunidade
			LEFT  JOIN entidade.endereco 	 		  	ed ON ed.endid = ep.endid
			LEFT  JOIN territorios.municipio		   	 m ON m.muncod = ed.muncod
		    INNER JOIN obras2.usuarioresponsabilidade 	ur ON ur.empid = ep.empid AND
															  ur.rpustatus = 'A'
			WHERE 
			 ".$where;
//	dbg($sql, d);
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			
			for ($i=0; $i<=$nlinhas;$i++) {
				
				foreach($RS[$i] as $k=>$v){ 
					${$k}=$v;
				}
								
				$orgao = recuperaOrgao($orgid);
				
	    		print " <option value=\"$orgid|$codigo\">{$obrid}|{$orgao}|{$descricao}</option>";
	    		
			}
		}
	} else{
		print '<option value="">Clique faça o filtro para selecionar.</option>';
		
	}
	
}

?>
  <html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Obras</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="../../includes/prototype.js"></script>
                <script type="text/javascript" src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		
		<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
		<table style="width:100%;" id="filtro" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="subtitulodireita">Tipo de Ensino:</td>
				<td>
				<?php
				
					if( $pflcod == PFLCOD_SUPERVISOR_UNIDADE ){
						$where = "AND orgid in ({$usuorgids})";
					}else{
						$where = "";
					}
					
					$sql = "SELECT
								orgid AS codigo, 
								orgdesc AS descricao
							FROM
								obras2.orgao
							WHERE
								orgstatus = 'A' {$where}";

					$db->monta_combo('orgid',$sql,'S',"-- Selecione para filtrar --",'limparCampos(0);filtroObras','','',170);
						
					echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';					
				?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">UF:</td>
				<td id="tdUF" >
				<? 
				if ($orgid){
					echo selectUF();
				}else{
				?>
					<select class="CampoEstilo" style="width:170px;" name="estuf" disabled="disabled">
						<option value=""> -- Selecione para filtrar -- </option>
					</select>
				<? 
				}
				?>	
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Unidade:</td>
				<td id="tdUnidade">
					<select class="CampoEstilo" style="width:170px;" name="unidade" disabled="disabled">
						<option value=""> -- Selecione para filtrar -- </option>
					</select>
				</td>
			</tr>			
			<tr id="trCampus" style="display:none">
				<td class="subtitulodireita">Campus:</td>
				<td id="tdCampus" >
					<select class="CampoEstilo" style="width:170px;" name="campus" disabled="disabled">
						<option value=""> -- Selecione para filtrar -- </option>
					</select>
				</td>
			</tr>
		</table>		
		<!-- Lista de Unidades -->
		<div id="tabela" style="overflow:auto; width:496px; height:270px; border:2px solid #ececec; background-color: #ffffff;">	
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Obra(s)</strong></td>		
					</tr>
				</thead>
			</table>
			<div id="listaObras">
			</div>
		</div>
		
<!-- Unidades Selecionadas -->
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<select multiple size="8" name="empresp[]" id="empresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'empresp' );" class="CampoEstilo" onchange="//moveto(this);">				
			<?php 
				buscaObrasCadastradas($usucpf, $pflcod);
			?>
		</select>
		
<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect); document.getElementsByName('gravar')[0].value=1; document.formulario.submit();" id="ok">
					<input type="hidden" name="gravar" value="">
				</td>
			</tr>
		</table>
</form>
            
<script type="text/JavaScript">
jQuery.noConflict();
function marcatudo(){
//    var checkPai = document.getElementById('checkPai');

//	for( i=0; i<=linhas; i++ ){
//		document.formulario.empid[i].checked = checkPai.checked;
//		retorna(i);
//	}
//    jQuery('.chkbox').prop('checked', !jQuery('#checkPai').prop('checked'));
//    jQuery('.chkbox').click();
    jQuery('.chkbox').each(function(i){
        jQuery(this).prop('checked', jQuery('#checkPai').prop('checked'))
        retorna(i);
    })
}
    
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.display  = 'block';


var campoSelect = document.getElementById("empresp");

<?
if ($funid):
?>
if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		var id = campoSelect.options[i].value.split('|');
		
		if (document.getElementById(id[1])){
			document.getElementById(id[1]).checked = true;
		}
	}
}
<?
endif;
?>

function retorna(objeto) {
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	var objObra    = document.getElementsByName('empid')[objeto];
	var objObraDsc = document.getElementsByName('empdsc')[objeto];
	if(objeto == 0){
		if (objObra.checked == true){
			campoSelect.options[tamanho] = new Option(objObraDsc.value, objObra.value, false, false);
			sortSelect(campoSelect);
		}
		else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (objObra.value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;}
				}
				if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Obra.', '', false, false);}
				sortSelect(campoSelect);
		}
	}else{
		if (objObra.checked == true){
			campoSelect.options[tamanho] = new Option(objObraDsc.value, objObra.value, false, false);
			sortSelect(campoSelect);
		}
		else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (objObra.value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;}
				}
				if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Obra.', '', false, false);}
				sortSelect(campoSelect);
		}
	}
}

//function selecionaTudo( linhas ){
//
//	var checkPai = document.getElementById('checkPai');
//
//	for( i=0; i<=linhas; i++ ){
//		document.formulario.empid[i].checked = checkPai.checked;
//		retorna(i);
//	}
//	
//}

function filtroObras(id) {

	var d 	    = document;
	var orgid   = d.getElementsByName('orgid')[0]   ? d.getElementsByName('orgid')[0].value   : '';
	var estuf   = d.getElementsByName('estuf')[0]   ? d.getElementsByName('estuf')[0].value   : '';;
	var unidade = d.getElementsByName('unidade')[0] ? d.getElementsByName('unidade')[0].value : '';;
	var campus  = d.getElementsByName('campus')[0]  ? d.getElementsByName('campus')[0].value  : '';;

	if( orgid == 3 || orgid == 5 ){
		d.getElementById('trCampus').style.display = 'none';
	}else{
		d.getElementById('trCampus').style.display = 'table-row';
	}
	
	if ( orgid ){
		if( estuf ){
			if( unidade ){
				if( campus || (orgid == 3) || (orgid == 5) ){
					
					new Ajax.Request(window.location.href,{
						method: 'post',
						parameters: '&requisicao=obras&orgid='+orgid+'&campus='+campus+'&unidade='+unidade,
						onComplete: function(res){
							d.getElementById('listaObras').innerHTML = res.responseText;
						}
					});	
				}else{
					new Ajax.Request(window.location.href,{
						method: 'post',
						parameters: '&requisicao=campus&orgid='+orgid+'&estuf='+estuf+'&unidade='+unidade,
						onComplete: function(res){
							d.getElementById('tdCampus').innerHTML = res.responseText;
						}
					});	
				}
			}else{
				new Ajax.Request(window.location.href,{
					method: 'post',
					parameters: '&requisicao=unidade&orgid='+orgid+'&estuf='+estuf,
					onComplete: function(res){
						d.getElementById('tdUnidade').innerHTML = res.responseText;
					}
				});	
			}
		}else{
			new Ajax.Request(window.location.href,{
				method: 'post',
				parameters: '&requisicao=uf&orgid='+orgid,
				onComplete: function(res){
					d.getElementById('tdUF').innerHTML = res.responseText;
				}
			});	
		}
	}
}

function limparCampos( nivel ){
	if ( nivel <= 0) {
		document.getElementsByName('estuf')[0].value='';
	}
	if ( nivel <= 1) {
		document.getElementById('tdUnidade').innerHTML = '<select class="CampoEstilo" style="width:170px;" name="unidade" disabled="disabled">'+'<option value=""> -- Selecione para filtrar -- </option>'+'</select>';
	}
	if ( nivel <= 2) {
		document.getElementById('tdCampus').innerHTML =  '<select class="CampoEstilo" style="width:170px;" name="campus" disabled="disabled">'+'<option value=""> -- Selecione para filtrar -- </option>'+'</select>';
	}
	if ( nivel <= 3) {
		document.getElementById('listaObras').innerHTML =  '';
	}
}
</script>
	</body>
</html>
