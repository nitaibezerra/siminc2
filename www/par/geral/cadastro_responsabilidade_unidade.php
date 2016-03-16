<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];
$orgid  = $_REQUEST['orgid'];
$estuf  = $_REQUEST['estuf'];
$muncod = $_REQUEST['muncod'];
$gravar = $_REQUEST['gravar'];
$unicod = $_REQUEST["uniresp"];

$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário

if ($_POST && $gravar == 1){
	atribuiUnidade($usucpf, $pflcod, $unicod, $orgid);
}


/**
 * Função que lista as unidades
 *
 */
function listaUnidades(){
	global $db, $funid, $estuf, $muncod, $orgid, $entid;
		
	$where  = array();
	$campo  = array();
	$from   = array();
	 
	$sql = "SELECT DISTINCT
				e.entid,
				iue.iuenome as entnome,
				mun.mundescricao,
				CASE WHEN iu.inuid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estuf,
				e.entcodent
			FROM 
				entidade.entidade e
			INNER JOIN par.instrumentounidadeentidade iue ON iue.entid = e.entid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = iue.inuid
			LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
			WHERE
				entstatus='A'
			ORDER BY 
				entnome";
	$unidadesExistentes = $db->carregar($sql);

	if (!$unidadesExistentes){
		echo "<tr>
				<td style='color:red;'>Busca não retornou registros...</td>
			  </tr>";
		return;
	}	
	
	$count = count($unidadesExistentes);
	
	$sql = "SELECT DISTINCT 
				iue.entid as codigo
			FROM
				par.instrumentounidadeentidade iue
			INNER JOIN 
				par.usuarioresponsabilidade ur ON iue.entid = ur.entid AND ur.rpustatus = 'A'
			WHERE  ur.usucpf = '{$_REQUEST['usucpf']}' AND 
			 	    ur.pflcod = {$_REQUEST['pflcod']}";
	
	$dadosSalvos = $db->carregarColuna($sql);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $unidadesExistentes[$i]["entid"];
		$descricao = $unidadesExistentes[$i]["entnome"];
		$municipio = $unidadesExistentes[$i]["mundescricao"];
		$codigoUf  = $unidadesExistentes[$i]["estuf"];
		$codinep   = $unidadesExistentes[$i]["entcodent"];
		$check = "";
		$orgid = 3;
		//dbg($codinep, 1);
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		if( in_array($codigo, $dadosSalvos) ){
			$check = "checked";
		}
		
		$html.= "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"unicod\" id=\"".$codigo."\" {$check} value=\"$orgid|$codigo\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"unidsc\" value=\"".($descricao)."\">
				</td>
				<td>
					".$descricao."
				</td>
			</tr>";
	}
	$html.= '<thead>
				<tr>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Total de Registros: '.sizeof($unidadesExistentes).'</strong></td>		
				</tr>
			</thead>';
	echo $html;
}

/**
 * Função que atribui a responsabilidade de uma unidade ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribuiUnidade($usucpf, $pflcod, $entid, $orgid = 'null'){

	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql_zera = $db->executar("UPDATE 
								par.usuarioresponsabilidade 
							   SET 
								rpustatus = 'I' 
							   WHERE 
								usucpf = '{$usucpf}' AND 
								pflcod = '{$pflcod}' AND 
								-- prsano = '{$_SESSION["exercicio"]}' AND 
								entid IS NOT NULL");
	
	if (is_array($entid) && !empty($entid[0])){
		$count = count($entid);
		
		// Insere a nova unidade
		$sql_insert = "INSERT INTO par.usuarioresponsabilidade (
							entid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod --,
							-- orgid , 
							-- prsano
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
			
			list($orgid,$entidade) = explode("|", $entid[$i]);
			
			if ( $entidade != $entidade_antiga ){
				$arrSql[] = "(
								'{$entidade}',
								'{$usucpf}', 
								'A', 
								'{$data}', 
								'{$pflcod}' --,
							--	'{$orgid}' -- , 
							--	'{$_SESSION["exercicio"]}'
							 )";
			}
			
			$entidade_antiga = $entidade;
			
		}

		$sql_insert = (string) $sql_insert.implode(",",$arrSql);
		$db->executar($sql_insert);
	}
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>");
	
}

function buscaUnidadesCadastradas($usucpf, $pflcod){
	
	global $db, $unicod;
	
	if (!$_POST['gravar'] && $_REQUEST["uniresp"][0]){
		foreach ($_REQUEST["uniresp"] as $v){
			list(,$entid[]) = explode('|', $v );
		}
		$where = " iue.entid IN (".implode(',',$entid).") ";
	}else{
		$where = " (ur.usucpf = '{$usucpf}' AND 
			 	    ur.pflcod = {$pflcod}) ";	
	}
	
	$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário
	
	if ( !$perfilSuperUser ){

		$sql = "select entid from par.usuarioresponsabilidade where usucpf = '{$_SESSION["usucpf"]}' AND pflcod = {$pflcod};";
		$entid = $db->pegaUm($sql);
		if($entid){
			$where = " iue.entid = $entid ";
		}
	}

	$sql = "SELECT DISTINCT 
				iue.entid as codigo, 
				iue.iuenome as descricao
			FROM
				par.instrumentounidadeentidade iue
		    INNER JOIN 
		    	par.usuarioresponsabilidade ur ON iue.entid = ur.entid AND
													ur.rpustatus = 'A'
			WHERE 
			 ".$where;
	
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			$arDescricao = array();
			for ($i=0; $i<=$nlinhas;$i++) {
				
				foreach($RS[$i] as $k=>$v){ 
					${$k}=$v;
				}
				if ($funid == 12){
					$orgid = 1;	
				}elseif($funid == 11 || $funid == 14 ){
					$orgid = 2;
				}elseif($funid == 16){
					$orgid = 5;
				}else{
					$orgid = 3;
				}
				
				
				//$orgdsc[$funid] = $orgdsc[$funid] ? $orgdsc[$funid] : recuperaOrgao($orgid);  
				if ( in_array("{$descricao}", $arDescricao) ){
					continue;
				}
				$arDescricao[]  = "{$descricao}";
				
				
	    		print " <option value=\"$orgid|$codigo\">{$descricao}</option>";
	    		
			}
		}
	} else{
		print '<option value="">Selecione a unidade.</option>';
		
	}
}

?><html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Unidades</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		
		<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
		<!-- Lista de Unidades -->
		<div id="tabela" style="overflow:auto; width:496px; height:290px; border:2px solid #ececec; background-color: #ffffff;">	
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						//document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidade(s)</strong></td>		
						</tr>
					</thead>
					<?php listaUnidades(); ?>
				</table>
		</div>
		<script language="JavaScript">
			document.getElementById('filtro').style.display = 'block';
		</script>
		<!-- Unidades Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
				<?php 
					buscaUnidadesCadastradas($usucpf, $pflcod);
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

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
//document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = 'block';


var campoSelect = document.getElementById("uniresp");

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


function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
}



function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
		if(document.formulario.unicod[objeto]) {
		if (document.formulario.unicod[objeto].checked == true){
			campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
			sortSelect(campoSelect);
		}
		else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (document.formulario.unicod[objeto].value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;}
				}
				if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);}
				sortSelect(campoSelect);
		}
	} else {
		// qunado possui apenas 1 registro
		if (document.formulario.unicod.checked == true){
			campoSelect.options[tamanho] = new Option(document.formulario.unidsc.value, document.formulario.unicod.value, false, false);
			sortSelect(campoSelect);
		}
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

function filtroFunid (id) {

	var d 	   = document;
	var orgid  = d.getElementsByName('orgid')[0]  ? d.getElementsByName('orgid')[0].value : '';
	var estuf  = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value : '';;
	var muncod = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';

	if (!orgid){
		alert('Selecione um "tipo de ensino" afim de efetuar o filtro!');
		return false;
	}
	
	selectAllOptions(campoSelect);
	d.formulario.submit();
	//window.location.href = '?pflcod=<?=$_GET['pflcod']; ?>&usucpf=<?=$_GET['usucpf']; ?>&funid='+funid+'&estuf='+estuf+'&muncod='+muncod;
}

function limpaMuncod(){
	if (document.getElementsByName('muncod')[0]) {
		document.getElementsByName('muncod')[0].value='';
	}
}
</script>
	</body>
</html>
