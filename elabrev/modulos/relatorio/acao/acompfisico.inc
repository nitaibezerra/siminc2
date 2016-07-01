<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:acompfisico.inc
   finalidade: Relatório de execução física das ações por período
   */

 //if (!$db->testa_superuser()) {print '<script>alert("Página em Construção!");history.back();</script>';exit();}
 //Aumenta a sessão da página

if ($_SESSION['sql_excel']){
	$nomeDoArquivoXls = "SIMEC_".date("His"); // Nome do arquivo excel
	$result = $db->record_set($_SESSION['sql_excel']);
	if (!$_SESSION['cab_excel']) {
	$count = pg_num_fields($result);
	for ($i = 0; $i < $count; $i++){$header .= '"'.str_replace('"', '""', pg_field_name($result, $i)).'"'."\t";}
	} else {
		for ($i = 0; $i < count($_SESSION['cab_excel']); $i++){$header .= '"'.str_replace('"', '""', $_SESSION['cab_excel'][$i]).'"'."\t";}
	}
	while($row = pg_fetch_assoc($result)){
	  $line = '';
	  foreach($row as $value){
	    if(!isset($value) || $value == "") $value = "\t";
	    else $value = '"' . str_replace('"', '""', $value) . '"' . "\t";
	  	$line .= $value;
	  }
	  $data .= trim($line)."\n";
	}
	$data = str_replace("\r", "", $data);
	if ($data == "") $data = "\nNão Foram Encontrados Registros\n";
	//Gera Arquivo
	header("Content-type: application/octet-stream");
	header("Content-Disposition: attachment; filename=$nomeDoArquivoXls.xls");
	header("Pragma: no-cache");
	header("Expires: 0");
	print $header."\n".$data; 
	$_SESSION['sql_excel']='';
	exit();
}
ini_set('session.gc_maxlifetime', 4800);

/*
$modulo=$_REQUEST['modulo'] ;
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];
*/
include "includes/cabecalho.inc";
print "<br>";
//Rola a tela para caber em 800x600
$ajustaTela = true;
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Relatório de Acompanhamento da Execução Física de Ações';
monta_titulo($titulo_modulo,'');

$prgid = $_REQUEST['prgid']; 
$acacod = $_REQUEST['acacod']; 
$unicod = $_REQUEST['unicod']; 
$regcod = $_REQUEST['regcod']; 
$refcod1 = $_REQUEST['refcod1']; 
$refcod2 = $_REQUEST['refcod2']; 
?>
<center>
<form method="POST"  name="formulario" style="margin:0px;">
<input type=hidden name="corcod_par" value="">
<input type=hidden name="numero" value="1">
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;<?=$largura?>">
      <tr>
        <td align='right' class="subtitulodireita">Programa:</td>
        <td><?$sql2 = "select distinct p.prgid as CODIGO, p.prgcod||' - '||prgdsc as DESCRICAO from programa p  inner join acao a on p.prgid=a.prgid and a.acasnrap='f' and a.acastatus='A' where prgstatus='A' and p.prgano='".$_SESSION['exercicio']."'  order by 2";
			$db->monta_combo("prgid",$sql2,'S',"Todos os Programas",'envia(1)','','',500);?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Ação:</td>
        <td><?
        $wh='';
        if ($prgid) $wh='and prgid='.$prgid;
        $sql2 = "select distinct acacod as CODIGO, acacod||' - '||acadsc as DESCRICAO from acao where 1=1 $wh and acasnrap='f' and acastatus='A' and prgano='".$_SESSION['exercicio']."' order by 2";
			$db->monta_combo("acacod",$sql2,'S',"Todas as Ações","envia(2)",false,false,500);?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Unidade Orçamentária:</td>
         <td>
         	<?
         	if ($acacod) $wh.=" and acacod='$acacod'";
			$sql2 = "select distinct a.unicod as CODIGO, a.unicod||' - '||u.unidsc as DESCRICAO from acao a inner join unidade u on a.unicod=u.unicod where 1=1 $wh order by 2";
			$db->monta_combo("unicod",$sql2,'S',"Todas as Unidades",$funcao_combo,false,false,500);
			?>
         </td>
    </tr>
    <tr>
        <td align='right' class="subtitulodireita">Região:</td>
         <td>
         	<?
			$sql2 = "select distinct a.regcod as CODIGO, a.regcod||' - '||r.regdsc as DESCRICAO from acao a inner join regiao r on a.regcod=r.regcod where 1=1 $wh order by 2";
			$db->monta_combo("regcod",$sql2,'S',"Todas as Regiões",$funcao_combo,false);
			?>
         </td>
    </tr>
	<tr><td align='right' class="SubTituloDireita">Período de Referência :</td>
		<td>
		<?$sql2 = "select refmes_ref as CODIGO, refmes_ref||'/'||refano_ref as DESCRICAO from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
			$db->monta_combo("refcod1",$sql2,'S',"",$funcao_combo,'');?>
		Até: 
		<?$sql2 = "select refmes_ref as CODIGO, refmes_ref||'/'||refano_ref as DESCRICAO from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t' and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
			$db->monta_combo("refcod2",$sql2,'S',"",$funcao_combo,'');?>
		</td>
	</tr>
	<tr><td align='right' class="SubTituloDireita"> </td>
		<td><input type="hidden" value="1" name="listar"><input type="button" name="lista" value="listar" onclick="envia(2);"></td>
	</tr>
</table>
</form>
</center>
		<?
	if ($_REQUEST['listar']){
		$wh='';
        if ($prgid) $wh=' and a.prgid='.$prgid;
        if ($acacod) $wh.=" and a.acacod='$acacod'";
        if ($unicod) $wh.=" and u.unicod='$unicod'";
        if ($regcod) $wh.=" and r.regcod='$regcod'";
        
		$sql = "select a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod||' - '||a.acadsc, u.unidsc, r.regdsc, pro.prodsc, 
		case when acasnmetanaocumulativa='t' then 'Não Cumulativa' else 'Cumulativa' end as acasnmetanaocumulativa,
		unm.unmdsc, 
		Coalesce((case when acasnmetanaocumulativa='t' then 
		(select max(exprealizado) from execucaopto where acaid=a.acaid and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2' and refano_ref='".$_SESSION['exercicio']."')) else 
		(select sum(exprealizado) from execucaopto where acaid=a.acaid and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2'  and refano_ref='".$_SESSION['exercicio']."')) end),'0') as executado,
		d.fisqtdeprevistoano
		from acao a
		inner join unidade u on a.unicod=u.unicod
		inner join regiao r on a.regcod=r.regcod
		inner join produto pro on a.procod=pro.procod and prodsc <> '-'
		inner join unidademedida unm on a.unmcod=unm.unmcod
		inner join dadofisico d on a.acaid=d.acaid
		where a.acasnrap='f' and a.prgano='".$_SESSION['exercicio']."' $wh order by 1";

		$cabecalho = array('Ação','Unidade','Região','Produto','Tipo','Unid. Medida','Realizado','Meta');
	
			$db->monta_lista($sql,$cabecalho,100,20,false,'','');
			$_SESSION['sql_excel'] = $sql;
			$_SESSION['cab_excel'] = $cabecalho;
	}?>

		
		
<script>
function envia(cod) {
	if (cod == 1) {document.formulario.acacod.value='';}
	document.formulario.submit();
}
</script>