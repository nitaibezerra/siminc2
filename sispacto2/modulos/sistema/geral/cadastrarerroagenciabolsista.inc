<?
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes.php";

function gravarBolsistasErroAgencia($dados) {
	global $db;
	
	$linhas = explode("
", $dados['listabolsistaerroagencia']);
	
	if($linhas) {
		foreach($linhas as $line) {
			if(trim($line)) {
				$colunas = explode("	", $line);
				
				$sql = "INSERT INTO sispacto2.bolsistaserroagencia(
					            cpf, nome, agencia)
					    VALUES ('".str_replace(array(".","-"),array("",""),$colunas[0])."', '".$colunas[1]."', '".$colunas[2]."');";
				
				$db->executar($sql);

				
			}
		}
		
		$db->commit();
	}
	
	$al = array("alert"=>"Lista gravada com sucesso","location"=>"sispacto2.php?modulo=sistema/geral/cadastrarerroagenciabolsista&acao=A");
	alertlocation($al);
	
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

monta_titulo( "Lista - Erros da agência de bolsistas", "&nbsp;");

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 

<form method="post" name="formulario_lista" id="formulario_lista">
<input type="hidden" name="requisicao" value="gravarBolsistasErroAgencia">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Lista</td>
	<td>
	<textarea name="listabolsistaerroagencia" cols="100" rows="20"></textarea>
	</td>
</tr>

<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" name="enviar" value="Gravar"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['dependencia']) {
	$f[] = "foo.dependencia='".$_REQUEST['dependencia']."'";
}

if($_REQUEST['estuf']) {
	$f[] = "foo.estuf='".$_REQUEST['estuf']."'";
}

$sql = "SELECT DISTINCT i.iuscpf, i.iusnome as nome, i.iusemailprincipal as email, trim(i.iusagenciasugerida) as ag1, CASE WHEN i.cadastradosgb=true THEN 'Sim' ELSE 'Não' END as cadastradosgb 
		from sispacto2.bolsistaserroagencia s
		inner join sispacto2.identificacaousuario i on i.iuscpf = trim(s.cpf)
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		where trim(i.iusagenciasugerida) = trim(s.agencia)";

$cabecalho = array("CPF","Nome","E-mail","Ag. Selecionada","Cadastrado no SGB");
$db->monta_lista($sql,$cabecalho,1000,10,'N','center','N','formulario');

?>