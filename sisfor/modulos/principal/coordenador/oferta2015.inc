<?



if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['sisfor']['iusd'] || ($_REQUEST['iusd'] && $_REQUEST['iusd'] != $_SESSION['sisfor']['iusd'])){
	$_SESSION['sisfor']['iusd'] = $_REQUEST['iusd'];
}

if($_SESSION['sisfor']['iusd']){
	$unicod = recuperarCodIES($_SESSION['sisfor']['iusd']);
}
	
if($unicod){
	$_SESSION['sisfor']['unicod'] = $unicod;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';

echo "<br>";

?>
<script>

function salvarOferta2015() {

	var embranco = false;
	
	jQuery("[name^='oferta[']").each(function() {
		if(jQuery.trim(jQuery(this).val())=='') {
			alert('Valor é obrigatório');
			jQuery(this).focus();
			embranco = true;
			return false;
		}
	});

	if(embranco) {
		return false;
	}
	

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "requisicao");
	input.setAttribute("value", "salvarOferta2015_IES");
	document.getElementById("formulariooferta").appendChild(input);

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "unicod");
	input.setAttribute("value", "<?=$_SESSION['sisfor']['unicod'] ?>");
	document.getElementById("formulariooferta").appendChild(input);
	
	document.getElementById("formulariooferta").submit();

}

function finalizarOferta2015() {
	var conf = confirm('Deseja realmente finalizar a oferta 2015?');

	if(conf) {
		window.location='sisfor.php?modulo=principal/coordenador/oferta2015&acao=A&requisicao=finalizarOferta2015';
	}
	
}

function somarTotal() {

	var total = 0;
	
	jQuery("[name^='oferta[']").each(function() {
		if(jQuery.trim(jQuery(this).val())!='') {
			total += parseInt(jQuery.trim(jQuery(this).val()));
		}
	});

	jQuery("#total").val(total);

	
}

<? if($db->testa_superuser()) : ?>
function reabrirOferta2015() {
	var conf = confirm('Deseja realmente reabrir a oferta 2015?');

	if(conf) {
		window.location='sisfor.php?modulo=principal/coordenador/oferta2015&acao=A&requisicao=reabrirOferta2015';
	}
}
<? endif; ?>

</script>
<?

$universidade = $db->pegaUm("SELECT uniabrev||' - '||unidsc as universidade FROM public.unidade WHERE unicod='".$_SESSION['sisfor']['unicod']."'");
$sieoferta2015ies    = $db->pegaUm("SELECT sieoferta2015ies FROM sisfor.sisfories WHERE unicod='".$_SESSION['sisfor']['unicod']."'");

monta_titulo( "Capacidade de Oferta para Matriz 2015", 
			  "<table width=100% cellspacing=1 cellpadding=3>
				<tr>
					<td class=subtituloCentro>".$universidade."</td>
				</tr>
				<tr>
					<td class=subtituloEsquerda>Orientações</td>
				</tr>
				<tr>
					<td>
		<p>Considerando a demanda por vagas para 2015, captada pelo PDDE Interativo junto às escolas públicas (dados disponíveis no endereço http://pddeinterativo.mec.gov.br), informe o número de vagas novas que sua instituição poderia ofertar com a Matriz Orçamentária de 2015. Nos números que você apresentará abaixo, não considere as vagas dos cursos em andamento, nem as de projetos submetidos à análise neste ano de 2014. Caso a Instituição não deseje ou não disponha de profissionais habilitados para ofertar o curso, indique 0 (zero). Nenhum campo poderá ficar em branco.</p>
		<p>Ao finalizar o preenchimento da tabela, clique em Finalizar Oferta 2015.</p>
		<p>Os valores indicados pela IES subsidiarão o FEPAD de sua UF na elaboração do Planejamento Articulado 2015 que será enviado ao Ministério da Educação. Cabe, em última análise, ao MEC, a elaboração do Projeto de Lei Orçamentária Anual, considerando os limites orçamentários.</p>
					</td>
				</tr>
				</table>");

$sql = "(
		SELECT DISTINCT
                cur.curid::text,
                cur.curdesc AS nomecurso,
                ncu.ncudesc AS nivelcurso,
                esp.espdesc AS etapaensino,
				array_to_string(array((select '<b>'||estuf||'</b> : <input type=\"text\" style=\"text-align:right;\" name=\"oferta['||estuf||']['||cur.curid||']\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select ofeqtdies::text from sisfor.oferta2015 where unicod='".$_SESSION['sisfor']['unicod']."' and curid=cur.curid and estuf=iu.estuf),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" id=\"oferta_'||cur.curid||'\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);somarTotal();\" ".(($sieoferta2015ies=='t')?" class=\"disabled\" readonly":" class=\" normal\"").">' FROM sisfor.oferta2015iesuf iu WHERE iu.unicod='".$_SESSION['sisfor']['unicod']."')),'<br>') as oferta2015

                FROM
                catalogocurso2014.curso cur
                LEFT JOIN catalogocurso2014.abrangenciacurso abc ON cur.curid = abc.curid
                INNER JOIN catalogocurso2014.cursorede crd ON crd.curid = cur.curid
                INNER JOIN catalogocurso2014.rede red ON crd.redid = red.redid
                LEFT JOIN catalogocurso2014.nivelcurso ncu ON ncu.ncuid = cur.ncuid
                INNER JOIN workflow.documento doc ON doc.docid = cur.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN catalogocurso2014.iesofertante ieo ON ieo.curid = cur.curid
                LEFT JOIN entidade.endereco een ON een.entid = ieo.entid
                LEFT JOIN catalogocurso2014.areatematica ate ON ate.ateid = cur.ateid
                LEFT JOIN catalogocurso2014.subarea sua ON sua.suaid = cur.suaid
                LEFT JOIN catalogocurso2014.especialidade esp ON esp.espid = cur.espid
                LEFT JOIN catalogocurso2014.usuarioresponsabilidade ur1 ON ur1.coordid = cur.coordid
                LEFT JOIN catalogocurso2014.usuarioresponsabilidade ur2 ON ur2.curid = cur.curid
                LEFT JOIN catalogocurso2014.localizacaoescola les ON cur.lesid = les.lesid 
		ORDER BY cur.curdesc
		) UNION ALL (
		SELECT '', '', '', '<span style=float:right><b>TOTAL:</b></span>', '<input type=\"text\" style=\"text-align:right;\" name=\"total\" size=\"6\" maxlength=\"5\" value=\"'||coalesce((select sum(ofeqtdies)::text from sisfor.oferta2015 where unicod='".$_SESSION['sisfor']['unicod']."'),'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'#####\',this.value);\" id=\"total\" title=\"Oferta 2015\" onkeyup=\"this.value=mascaraglobal(\'#####\',this.value);\" class=\"disabled\" readonly>' as totaloferta2015
		)";

$cabecalho = array("Código","Nome do curso","Nível do curso","Especialidade","UF : Oferta 2015");
$db->monta_lista($sql,$cabecalho,1000,10,'N','center','N','formulariooferta');

if($sieoferta2015ies!='t') :
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloDireita">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
</table>
<?
elseif($db->testa_superuser()) :
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloDireita">&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
</table>
<?
endif;
?>