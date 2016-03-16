<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Adonias Malosso
   Programador: Adonias Malosso (e-mail: malosso@gmail.com)
   módulo:monitoremenda.inc
   finalidade: permitir o monitoramento de emendas
   */
$modulo=$_REQUEST['modulo'] ;

if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid'];
	else $_REQUEST['prgid'] = $_SESSION['prgid'];

if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; 
	else $_REQUEST['acaid'] = $_SESSION['acaid'];
//
// remover lançamento
if ((int)$_REQUEST['removerlibid']) {
	$libid = $_REQUEST['removerlibid'];
	$sql = "SELECT * FROM monitora.emendaliberacao WHERE libid = $libid";
	$linhas = $db->carregar($sql);
	if(count($linhas)==1) {
		$liberacao = $linhas[0];
		$sql = "UPDATE monitora.emendaliberacao SET libremovido = 't', libdataremoveu = NOW(), libusucpfremoveu = '" . $_SESSION["usucpforigem"] . "' WHERE libid = " . $liberacao["libid"];
		$db->executar($sql);
		$db->commit();
	}
	$db->sucesso($modulo);
	exit();
}
//
//Faz as Validações de Campo Obrigatório
if ((int)$_REQUEST['emdidliberar'])
{
	$erro = "";
	$sqlSelEmenda = "SELECT emdid FROM monitora.emenda WHERE emdid = '%d'";	
	$sqlSelValorLiberacao = "SELECT emdsaldo - (SELECT COALESCE(SUM(libvalor),0) FROM monitora.emendaliberacao WHERE emendaliberacao.emdid = emenda.emdid AND emendaliberacao.libremovido = 'f') FROM monitora.emenda WHERE emenda.emdid = '%d'";
	$sqlInsLiberacao = "INSERT INTO monitora.emendaliberacao (emdid, libdocumento, libdata, libvalor, libdatainc, libusucpf) VALUES ('%s', '%s', '%s', '%s', NOW(), '%s')";
	
	// variaveis enviadas
	$emdid = (int)$_REQUEST["emdidliberar"];
	$documento = $_REQUEST["documento_$emdid"];
	$data = $_REQUEST["data_$emdid"];
	$valor = $_REQUEST["valor_$emdid"];
	
	//
	// todos os campos preenchidos
	if(!$emdid || !$documento || !$data || !$valor) {
		$erro .= 'Preencha todos os campos.\n';
	}
	
	//
	// verificar se a emenda existe
	$sql = sprintf($sqlSelEmenda, $emdid);
	if(!$db->pegaUm($sql)) {
		$erro .= 'Emenda inválida.\n';
	}
	
	//
	// data valida
	$data = explode("/", $data);
	if((@count($data)==3) && (!checkdate($data[1], $data[0], $data[2]))) {
		$erro .= 'A data informada está inválida.\n';
	}
	$data = $data[2]."-".$data[1]."-".$data[0];

	//
	// valor
	$valor = str_replace(".", "", $valor);
	$valor = str_replace(",", ".", $valor);
	
	//
	// verificar limite disponivel
	//
	// LEMBRAR DE VERIFICAR O TOTAL DISPONIVEL EM TODOS OS GNDS DESSE PARLAMENTAR

	$sql = sprintf($sqlSelValorLiberacao, $emdid);
	$limite = (int)$db->pegaUm($sql);
	if($valor>$limite) {
		$erro .= sprintf('O valor informado (R$ %s) é maior do que o limite disponível (R$ %s)\n'
							,number_format($valor, 2, ",", ".")
							,number_format($limite, 2, ",", "."));	
	}
	if ($erro) {
		?>
<script language="Javascript">
	alert('<?='Foram encontrados o(s) seguinte(s) erro(s):\n'.$erro?>');
	history.back();
</script>
		<?
	}
	else {
		$sql = vsprintf($sqlInsLiberacao, array($emdid, $documento, $data, $valor, $_SESSION["usucpforigem"]));
		$db->executar($sql);
	    $db->commit();
		$db->sucesso($modulo);
	}
	exit();
}
include APPRAIZ."includes/cabecalho.inc";
?>
<script type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<script type="text/javascript">
function confirmaRemoverLiberacao(libid) {
	if(confirm("Confirma remover lançamento de recurso liberado?")) {
		apagaLiberacao(libid, document.getElementById('formremoverliberacao'));
	}
	else {
		return false;
	}
}
</script>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Monitorar Emendas';
monta_titulo($titulo_modulo,'');

$sql= "SELECT a.*, u.* FROM acao a INNER JOIN unidade u ON a.unicod = u.unicod WHERE a.acastatus = 'A' AND a.acaid = " . $_REQUEST['acaid'];
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas < 0 ){
	?>
	<script>alert('Ação Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
$res =  $db->carrega_registro($RS,0);
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if (! $_REQUEST['refcod'])
{
	$sql = "SELECT refcod FROM referencia 
	WHERE refsnmonitoramento='t' AND refdata_inicio < now()::date
		AND refano_ref='".$_SESSION['exercicio']."' 
	ORDER BY refano_ref DESC, refmes_ref DESC";

	$RS = $db->record_set($sql);
	$res =  $db->carrega_registro($RS,0);
	$_REQUEST['refcod']= $res[0];
}
?>
<form id="formremoverliberacao" name="formremoverliberacao" method="post">
<input type="hidden" name="removerlibid" value=""/>
</form>
<form name="formulario" method="post">
<input type="hidden" name="emdidliberar" value=""/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='center' bgcolor="#dcdcdc" colspan="2"><b>Ação</b></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acadsc?></td>
      </tr>
	  </tr>
        <td align='right' class="subtitulodireita">Unidade:</td>
        <td><?=$unicod.' - '.$unidsc?></td>
      </tr>
	  </tr>
        <td align='right' class="subtitulodireita">Programa de Trabalho Resumido:</td>
        <td><?=$acaptres?></td>
      </tr>
      <?
$sql = "SELECT pfl.pfldsc AS descricao, 
			pfl.pflsncumulativo AS mostra, 
			usu.usunome || ' ('|| org.orgdsc ||')' AS usuario
		FROM perfil pfl
			LEFT JOIN usuarioresponsabilidade rpu ON rpu.pflcod = pfl.pflcod 
				AND rpu.acaid = " . $_SESSION['acaid'] . "
				AND rpu.rpustatus='A'
			LEFT JOIN usuario usu ON usu.usucpf = rpu.usucpf 
			LEFT JOIN orgao org ON org.orgcod = usu.orgcod
		WHERE pfl.pflstatus='A' AND pfl.pflresponsabilidade in ('A','X','D')
		ORDER BY pfl.pfldsc";
$RS2 = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS2);
if ($nlinhas >= 0) {
    for ($i=0; $i<=$nlinhas; $i++) {
        $res = $db->carrega_registro($RS2,$i);
		if ((!$res["mostra"] or $res["mostra"]=='f') and $res["usuario"] ) {
			echo "<tr><td align='right' class='SubTituloDireita'>" . $res["descricao"] . ":</td><td>" . $res["usuario"] . "</td></tr>";
		}
	}
}

//
// dados das emendas
$sqlEmenda = "SELECT emdid, emdnomeautor, emdsglpartidoautor, 
emdufautor, foncod, gndcod, mapcod, emdsaldo AS saldo, (SELECT COALESCE(SUM(libvalor),0) FROM monitora.emendaliberacao WHERE emendaliberacao.emdid = emenda.emdid AND emendaliberacao.libremovido = 'f' ) AS liberado
FROM monitora.emenda WHERE acaid = $acaid ORDER BY emdnomeautor, emdsglpartidoautor, emdufautor";
$sqlLiberado = "SELECT l.*, u.usunome FROM monitora.emendaliberacao l INNER JOIN seguranca.usuario u ON u.usucpf = l.libusucpf WHERE l.emdid IN (SELECT emdid FROM monitora.emenda WHERE acaid = $acaid) AND l.libremovido = 'f' ORDER BY l.libdata";
$linhas = $db->carregar($sqlEmenda);
$liberados = $db->carregaAgrupado($sqlLiberado, "emdid");

//
// exibir resumo de emendas
?>
      <tr>
        <td align='center' bgcolor="#dcdcdc" colspan="2"><b>Emendas (<?=@count($linhas)?>)</b></td>
      </tr>
      <tr>
        <td colspan="2" align='center'>
<?
if(!is_array($linhas) || @count($linhas)<=0) {
	?>
        <font color="Red"><b>Não foram encontrados registros</b></font>
	<?
}
else {
	?>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
          <thead>
          <tr>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Autor</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Partido</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Fonte</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">GND</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Mod. de Aplicação</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Saldo</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Liberado</td>
			<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação</td>
		  </tr>
		  </thead>
		  <tbody>
	<?
	$total = array();
	$total["liberado"] = 0;	
	$total["saldo"] = 0;

	$i = -1;
	$imgs[++$i]["src"] = "../imagens/valida0.gif";
	$imgs[$i]["alt"] = "Nenhum";
	$imgs[$i]["status"] = 0;
	$imgs[++$i]["src"] = "../imagens/valida1.gif";
	$imgs[$i]["alt"] = "Liberação completa";
	$imgs[$i]["status"] = 3;	
	$imgs[++$i]["src"] = "../imagens/valida2.gif";
	$imgs[$i]["alt"] = "Liberação parcial";
	$imgs[$i]["status"] = 2;	
	$imgs[++$i]["src"] = "../imagens/valida3.gif";
	$imgs[$i]["alt"] = "Pendente";
	$imgs[$i]["status"] = 1;
	foreach($linhas as $linha) {
		$emdid = $linha["emdid"];
		$data[$emdid] = date("d/m/Y");

		$total["liberado"] += $linha["liberado"];
		$total["saldo"] += $linha["saldo"];
		
		// dotacao atual - liberado
		$pendente = $linha["saldo"] - $linha["liberado"];
		if(!$linha["liberado"]) {
			$numStatus = 3;
		}
		elseif($pendente>0) {
			$numStatus = 2;
		}
		elseif($pendente==0) {
			$numStatus = 1;
		} 
		else {
			$numStatus = 0;
		}
		$img = $imgs[$numStatus];
		$marcado = $i++ % 2 ? '#E9E9E9' : '';
	?>
		  <tr onclick="abreconteudo('<?=$emdid?>')" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td title="Autor"><img src="../imagens/mais.gif" name="+" border="0" id="img-<?=$emdid?>"> <?=$linha["emdnomeautor"]?></td>
			<td align="center" title="Partido"><?=$linha["emdsglpartidoautor"]?></td>
			<td align="center" title="UF"><?=$linha["emdufautor"]?></td>
			<td align="right" style="color:#999999;" title="Fonte"><?=$linha["foncod"]?></td>
			<td align="right" style="color:#999999;" title="GND"><?=$linha["gndcod"]?></td>
			<td align="right" style="color:#999999;" title="Mod. de Aplicação"><?=$linha["mapcod"]?></td>			
			<td align="right" style="color:#999999;" title="Saldo"><?=number_format($linha["saldo"], 2, ",", ".")?></td>
			<td align="right" style="color:#999999;" title="Liberado"><?=number_format($linha["liberado"], 2, ",", ".")?></td>
			<td align="center" style="color:#999999;" title="Situação"><img src="<?=$img["src"]?>" title="<?=$img["alt"]?>" border="0"/></td>
		  </tr>
		  <tr bgcolor="<?=$marcado?>">
		    <td colspan="10" id="td-<?=$emdid?>" style="display: none; visibility: hidden; border-bottom: 1px solid #333333">
		    <img src="../imagens/seta_filho.gif" border="0" align="left"/>
		    <table align="right" border="0" cellpadding="2" cellspacing="0" style="border-top: 1px solid #fcfcfc;">
		      <tr>
				<td align='right' class='subtitulodireita' title="Nr. Documento">Nº Documento</td>
				<td align='right' class='subtitulodireita' title="Data de Liberação">Data</td>
				<td align='right' class='subtitulodireita' title="Valor Liberado">Valor (R$)</td>
				<td class='subtitulodireita'>&nbsp;</td>
		      </tr>
<?
if(is_array($liberados[$emdid]) && @count($liberados[$emdid])>0) {
	$j = 0;
	foreach($liberados[$emdid] as $liberado) {
		$marcado2 = $j++ % 2 ? '#E3E3E3' : '#EEEEEE';		
		?>
			  <tr bgcolor="<?=$marcado2?>">
				<td align="center"><?=$liberado["libdocumento"]?></td>
				<td align="center"><?=date("d/m/Y", strtotime($liberado["libdata"]))?></td>
				<td align="right"><?=number_format($liberado["libvalor"], 2, ",", ".")?></td>
				<td align="center" rowspan="2"><a href=Javascript:confirmaRemoverLiberacao(<?=$liberado["libid"]?>)>remover</a></td>
		      </tr>
		      <tr bgcolor="<?=$marcado2?>">
		        <td colspan="3" align="right">Liberado por <i><?=ucwords(strtolower($liberado["usunome"]))?></i> em <i><?=date("d/m/Y H:i:s", strtotime($liberado["libdatainc"]))?></i></td>
		      </tr>
	    <?
	}
}
if($img["status"]<3) {
	$marcado2 = $j++ % 2 ? '#E3E3E3' : '#EEEEEE';
?>
			  <tr bgcolor="<?=$marcado2?>">
				<td align='right'><?=campo_texto("documento_$emdid", 'N', 'S', 'Nr. Documento', '15', '10', '', 'N', 'right')?></td>
			    <td align='right'><?=campo_data("data_$emdid", 'N', 'S', 'Data de liberação', 'S')?></td>
				<td align='right'><?=campo_texto("valor_$emdid", 'N', 'S', 'Valor Liberado', '20', '14', '###.###.###,##', 'N', 'right')?></td>
				<td align='center' title="inserir"><input type="button" name="inserir_<?=$emdid?>" value="Liberar" onClick="enviaForm(<?=$emdid?>, this.form)"/></td>
		      </tr>
<?
}
?>
			</table></td>
		  </tr>
	<?
	}
	?>
		  </tbody>
		  <thead>
		  <tr>
			<td align="right" title="Partido">Totais:&nbsp;&nbsp;&nbsp;</td>
			<td align="right" title="UF"></td>
			<td align="right" title="Autor"></td>
			<td align="right" title="Fonte"></td>
			<td align="right" title="GND"></td>
			<td align="right" title="Mod. de Aplicação"></td>			
			<td align="right" title="Saldo"><?=number_format($total["saldo"], 2, ",", ".")?></td>
			<td align="right" title="Liberado"><?=number_format($total["liberado"], 2, ",", ".")?></td>
			<td align="right" title="Situação"></td>
		  </tr>
		  </thead>
		</table></td>
	<?
}
?>
      </tr>
</table>
<br>
</form>
<script language="javascript">
function abreconteudo(idobjeto)
{
if (document.getElementById('img-'+idobjeto).name=='+')
	{
	document.getElementById('img-'+idobjeto).name='-';
    document.getElementById('img-'+idobjeto).src = document.getElementById('img-'+idobjeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById('td-'+idobjeto).style.visibility = "visible";
	document.getElementById('td-'+idobjeto).style.display  = "";
	}
	else
	{
	document.getElementById('img-'+idobjeto).name='+';
    document.getElementById('img-'+idobjeto).src = document.getElementById('img-'+idobjeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById('td-'+idobjeto).style.visibility = "hidden";
	document.getElementById('td-'+idobjeto).style.display  = "none";
	}
}
</script>
<script>
function enviaForm(emdid, form) {
	form.emdidliberar.value = emdid;
	form.submit();
}
function apagaLiberacao(libid, form) {
	form.removerlibid.value = libid;
	form.submit();
	
}
</script>