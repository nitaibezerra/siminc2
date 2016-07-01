<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitoraacao.inc
   finalidade: permitir o monitoramento de Ação
   */
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];


if ($_REQUEST['tipograva']== 'A' and $_REQUEST['avptexto']=='100000')
{
  $erro_total=0;
  foreach($_POST as $k=>$v)
  {
    if (substr($k,0,10)=='avaliacao_')
     {
        $erro = 0;
        ${$k}=$v;
        $parte = substr($k,10,strlen($k)-10);
        $refer = substr($parte,0,strpos($parte,'_'));
        $parte2 = substr($parte,strpos($parte,'_')+1,100);
        $codigo = substr($parte2,0,strpos($parte2,'_'));
        $ordem = substr($parte2,strpos($parte2,'_')+1,100);

        $texto = str_replace("'","''",$texto);
        
        $cor =  $_REQUEST['corcodav'.$ordem];
	    $sit =  $_REQUEST['tpscodav'.$ordem];
	    $exp =  $_REQUEST['exprealizado'.$ordem];
	    $texto = stripslashes($_REQUEST['avaliacao_'.$refer.'_'.$codigo.'_'.$ordem]);
    	$texto = str_replace("'","''",$texto);
        //$texto = strip_tags($texto,'<p>,<b>,<i');
        $texto = stripslashes($texto);

	    if (ereg_replace("<[^>]*>","",$texto) == '') {$erro = 1;$erro_total=1;};
        if ($exp == '') {$erro = 1;$erro_total=1;};
        if ($sit == '') {$erro = 1;$erro_total=1;};
        if ($cor == '') {$erro = 1;$erro_total=1;};

        if (! $erro)
        {
           if ($codigo == 0)
               $sql= "insert into avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,acaid,avptexto,avpstatus,avpdata) values (1,1,".$refer.",'".$_SESSION['usucpf']."',".$_POST['corcodav'.$ordem].",".$_POST['tpscodav'.$ordem].",".$_SESSION['acaid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
           else
               $sql= "update avaliacaoparecer set refcod=".$refer.",usucpf='".$_SESSION['usucpf']."',corcod=".$_POST['corcodav'.$ordem].",tpscod=".$_POST['tpscodav'.$ordem].",acaid=".$_SESSION['acaid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$codigo;
           $db->executar($sql);
           if ($_POST['exprealizado'.$ordem] <>'-')
           {
			  $sql = "select * from execucaopto where refcod=".$refer." and acaid=".$_SESSION['acaid'];
		     if($db->eof($sql))
		     {
			    $sql = "insert into execucaopto (exprealizado, usucpf, acaid, refcod) values (".$_POST['exprealizado'.$ordem].",'".$_SESSION['usucpf']."',".$_SESSION['acaid'].",".$refer.")";
		     }
             else
		     {
       		     $sql = "update execucaopto set exprealizado=".$_POST['exprealizado'.$ordem].", usucpf='".$_SESSION['usucpf']."', expdata='".date('Y/m/d')."' where acaid=".$_SESSION['acaid']." and refcod=".$refer;
	         }
		     $db->executar($sql);
	       }
         }
     }
  }
  if ($erro_total)
  {
	   ?>
	      <script>
	         alert ('Há campos vazios que não foram gravados.\nA avaliação, a situação e a cor não podem ficar em branco.\nVerifique os dados.\nSomente registros completos são gravados corretamente.!');
	      </script>
	   <?
  }
  if (! $erro or $_REQUEST['refcod']=='x')
	{
       $db->commit();
       $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	   exit();
	}
}

if ($_REQUEST['tipograva']== 'LA' or ($_REQUEST['tipograva']== 'A' and $_REQUEST['avptexto']<>'100000'))
{
	$cor =  $_REQUEST['corcodav'.$_REQUEST['avptexto']];
	$sit =  $_REQUEST['tpscodav'.$_REQUEST['avptexto']];
	$exp =  $_REQUEST['exprealizado'.$_REQUEST['avptexto']];
	$texto = stripslashes($_REQUEST['avaliacao_'.$_REQUEST['refren'].'_'.$_REQUEST['doccod'].'_'.$_REQUEST['avptexto']]);
	$texto = str_replace("'","''",$texto);
//	$texto = strip_tags($texto);
    if (ereg_replace("<[^>]*>","",$texto) == '') {
	   ?>
	      <script>
	         alert ('A Avaliação não pode estar em branco.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
	   if ($exp == '') {
	   ?>
	      <script>
	         alert ('Informe o Executado no Período. Campo Obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
       if ($sit == '') {
	   ?>
	      <script>
	         alert ('Escolha uma situação. Campo obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
       if ($cor == '') {
	   ?>
	      <script>
	         alert ('Escolha uma das cores. Campo Obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
		if ($_REQUEST['tipograva']== 'A')
		{
		     // é uma avaliação
		     // verifico se o cod é zero, se for, será um insert, senão será update
		    if ($_REQUEST['doccod']== '0')
		     {
		       $sql= "insert into avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,acaid,avptexto,avpstatus,avpdata) values (1,1,".$_REQUEST['refren'].",'".$_SESSION['usucpf']."',".$cor.",".$sit.",".$_SESSION['acaid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
			   //print $sql.'<br>';
			   $db->executar($sql);
		     }
		     else
		     {
		       $sql= "update avaliacaoparecer set refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",acaid=".$_SESSION['acaid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
			   //print $sql.'<br>';
			   $db->executar($sql);

		     }
		}
		if ($_REQUEST['tipograva']== 'LA')
			{
      $sql= "update avaliacaoparecer set avpliberada='t',refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",acaid=".$_SESSION['acaid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 }
		//Grava o executado
		if ($exp<>'-')
		{
			 $sql = "select * from execucaopto where refcod=".$_REQUEST['refren']." and acaid=".$_SESSION['acaid'];
			 if($db->eof($sql))
			{
				$sql = "insert into execucaopto (exprealizado, usucpf, acaid, refcod) values (".$exp.",'".$_SESSION['usucpf']."',".$_SESSION['acaid'].",".$_REQUEST['refren'].")";
				$db->executar($sql);
			}else
			{
				$sql = "update execucaopto set exprealizado=".$exp.", usucpf='".$_SESSION['usucpf']."', expdata='".date('Y/m/d')."' where acaid=".$_SESSION['acaid']." and refcod=".$_REQUEST['refren'];
				$db->executar($sql);
			}
		}
	 $db->commit();
	 $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	 exit();
}
if ($_REQUEST['tipograva']== 'BA')
{
     $sql= "update avaliacaoparecer set avpliberada='f' where avpid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 $db->commit();
	 $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	 exit();
}

include "includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Avaliação de Ação';
monta_titulo($titulo_modulo,'');
$sql= "select a.*, u.*, m.unmdsc, p.prodsc from acao a inner join unidade u on a.unicod=u.unicod left join unidademedida m on a.unmcod=m.unmcod left join produto p on a.procod=p.procod where a.acastatus='A' and a.acaid = ".$_REQUEST['acaid'];
if (! $_REQUEST['acaid'] or $_REQUEST['acaid']=='') {
	?>
	<script>alert('Ocorreu um problema ao abrir a página. Tente novamente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}


$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

if ($nlinhas < 0 ){
	?>
	<script>alert('Ação Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  $db->carrega_registro($RS,0);
	//if ($res['acasnrap']=='t') {
?>
	<script>//alert('Esta ação é de Restos a Pagar e não é monitorada!');
//	history.back();
</script>
	<?
	//$db->close();
//	exit();
//}
}
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if (! $_REQUEST['refcod'])
{
  $sql = "select refcod from referencia where refsnmonitoramento='t' and refdata_inicio < now()::date and refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc";
$RS = $db->record_set($sql);
$res =  $db->carrega_registro($RS,0);
$_REQUEST['refcod']= $res[0];
}
?>
<form name="formulario" method="post">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="refren" value=0>
<input type=hidden name="doccod" >
<input type=hidden name="tipograva">
<input type=hidden name="procura">
<input type=hidden name="avptexto" value=0>

<center>
<script language="javascript" type="text/javascript" src="includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
//tinyMCE.init({
//	mode : "textareas",
//	theme : "advanced",
//	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
//	theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, print,preview,fullscreen,separator",
//	theme_advanced_buttons2 : "",
//	theme_advanced_buttons3 : "",
//	theme_advanced_toolbar_location : "top",
//	theme_advanced_toolbar_align : "left",
//	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
//	language : "pt_br",
//	entity_encoding : "raw",
//	width : "100%"
//});
</script>
<!--<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">-->
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acadsc?></td>
      </tr>
	  </tr>
        <td align='right' class="subtitulodireita">Unidade:</td>
        <td><?=$unicod.' - '.$unidsc?></td>
      </tr>
<?
   $sql = "select pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || ' ('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.acaid = ".$_SESSION['acaid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('A','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

            if ((!$mostra or $mostra=='f') and $usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
        }
     }
     

?>
<tr><td align='right' class="SubTituloDireita">Período de Referência :</td>
<td>
<?
$refcod = $_REQUEST['refcod'];
$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref desc";
$db->monta_combo("refcod",$sql2,'S',"Escolha o período de referência",'Escolhe_referencia()','Todos os Períodos de Referência');
?>
</td></tr>
  </table>
  <?/////////////////////Grafico evolucão produto
//verifica se pode ser acompanhado (Se tem produto pode!) nao pode o procod 0,3
if (!in($procod,array('0','3'))) {
	//Recupera os dados da ação
	  $sql = "SELECT aca.acasnmetanaocumulativa, aca.acaid, aca.acacod, aca.procod, pro.prodsc, 
dad.fisqtdeprevistoano as previsto, u.unmdsc,
case when aca.acasnmetanaocumulativa='f' then sum(exe.exprealizado) else max(exe.exprealizado) end as realizado
 FROM acao aca 
left join unidademedida  u on u.unmcod=aca.unmcod 
left join produto pro on aca.procod=pro.procod 
left  join dadofisico dad on aca.acaid=dad.acaid 
left join execucaopto exe on aca.acaid=exe.acaid
WHERE aca.acaid=".$_SESSION['acaid']." and aca.acasnrap='f' group by aca.acasnmetanaocumulativa, aca.acaid, aca.acacod, aca.procod, pro.prodsc, 
dad.fisqtdeprevistoano, u.unmdsc";

	  $RS = $db->record_set($sql);
	  $nlinhas = $db->conta_linhas($RS);
	  if ($nlinhas >= 0)
	  {

	  	for ($i=0; $i<=$nlinhas;$i++){
			$res = $db->carrega_registro($RS,$i);
		  	foreach($res as $k=>$v) ${$k}=$v;}
	  }
	  
	  $porcentorealizado = 0;
	  $porcentoexecutado = 0;
	  $porcentoplanejado = 0;
	  $planejado = 0;
	  $fisqtdeprevistoano = $previsto;
	  
	  if ($acasnmetanaocumulativa=='t')
		  	{ // é não cumulativa
               $tipometa = 'Não cumulativa';
	        } else
	        {  // é cumulativa
                $tipometa = 'Cumulativa';
	        }
	 
	  if ($previsto>0) {
	    $porcentorealizado = $realizado*100/$previsto;
	    if ($porcentorealizado > 100) $porcentorealizado2 = 100;
	    else $porcentorealizado2 = $porcentorealizado;
	    $porcentorealizado  = number_format($porcentorealizado, 2, '.', '');
	    }
	  $porcentoplanejado = 0;
	  $planejado = 0;
	  $porcentoexecutado = 100 - $porcentorealizado;
	   ?>
	   <TABLE border="0" cellspacing="0" cellpadding="2" class="listagem" align="center" width="95%">
	<THEAD>
		<tr>
		
		<TD colspan="13" align="center" style="color:#990000;">Produto da Ação: <strong><?=$prodsc.'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    (Meta: '.$tipometa.')'?></strong><?if ($db->testa_uma() or $db->testa_coordenador($_SESSION['acaid'],'A')){?><img border="0" src="imagens/alterar.gif" title=" Alterar meta" onclick="edita_dadacao()">&nbsp;&nbsp;<?}?></TD>
		<TD colspan="3" align="right">Unid. Medida:<strong><?=$unmdsc?></strong><br>
		<table cellpadding="0" cellspacing="0"><tr><td nowrap>0% <label style="border: 1px solid #000000; font-size: 8px; border-top: 1px solid #c0c0c0; border-right: 1px solid #c0c0c0; border-left: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0;background-color:#ffffff;" title="Realizado <?=number_format($porcentorealizado, 2, ',', '.');?>%"><span style="background-color:#<?if ($porcentorealizado>100) print "ff0000"; else print "0000ff";?>;font-size: 8px;"><?for ($k=0; $k<$porcentorealizado2/2;$k++) print "&nbsp;";?></span><?for ($k=0; $k<$porcentoexecutado/2;$k++) print "&nbsp;";?></label> <?if ($porcentorealizado>100) print number_format($porcentorealizado, 0, '.', '')."%"; else print "100%";?></td></tr></table></TD>
		</tr>
	</THEAD>
	<TBODY>
	<TBODY>
			    <TR style="color:#808080;" bgcolor="#f6f6f6">
				<td></td>
				<?$sql = "select refmes_ref, refano_ref from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
					$RS2 = $db->record_set($sql);
					$nlinhas2 = $db->conta_linhas($RS2);
					if ($nlinhas2 >= 0) {
						for ($j=0; $j<=$nlinhas2;$j++){
							$res2 = $db->carrega_registro($RS2,$j);
						  	foreach($res2 as $k=>$v) ${$k}=$v;?>
	    			<TD align="right" width="55"><?=$refmes_ref.'/'.$refano_ref?></TD>
				<?		}
					}
					//$colspan = 14 - $j;
					//if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
				?>
				<td align="right" colspan="2">Realizado</td>
				<td align="right" colspan="2">META</td>
			   </TR>
	
			   
			    <TR style="height:30px;" bgcolor="#ffffff">
				<TD align="right"><font color="#3366ff">Realizado</font></TD>
				<?
				  $sql = "select r.refcod, e.exprealizado, e.expobs, e.tpscod, t.tpscod, case when t.tpsdsc is null then 'Não Informado' else t.tpsdsc end as tpsdsc, case when c.cordsc is null then 'branco' else trim(c.cordsc) end as cordsc from referencia r  left join avaliacaoparecer a on r.refcod=a.refcod and a.acaid=".$_SESSION['acaid']." and a.avpliberada='t' left join execucaopto e on r.refcod=e.refcod and e.acaid=".$_SESSION['acaid']." left join tiposituacao t on a.tpscod=t.tpscod left join cor c on a.corcod=c.corcod where r.refdata_limite_avaliacao_aca is not null and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."' order by refano_ref,refmes_ref";

				$RS2 = $db->record_set($sql);
				$nlinhas2 = $db->conta_linhas($RS2);
				$totalrealizado = 0;
				if ($nlinhas2 >= 0) {
					for ($j=0; $j<=$nlinhas2;$j++){
						$res2 = $db->carrega_registro($RS2,$j);
					  	foreach($res2 as $k=>$v) ${$k}=$v;
						if (!$exprealizado) $txtexprealizado = '-'; else $txtexprealizado = $exprealizado;?>
				<td align="right" style="color:#3366ff;" title="<?=$tpsdsc?>" onClick="location.href='simec.php?modulo=principal/ppafasequalitativa/acao/monitoraacao&acao=A&refcod=<?=$refcod?>';" onMouseOver="this.bgColor='#ffffcc';" onMouseOut="this.bgColor='';">&nbsp;<?=formata_numero($txtexprealizado)?><br>
				<img border="0" width="25" height="10" src="imagens/av_<?=strtolower($cordsc)?>.gif"></td>
				<?			}
					}
				?>
			    <TD align="right" style="color:#3366ff;" colspan="2"><strong><?=formata_numero($realizado);
?></strong></TD>
				<TD align="right" style="color:#3366ff;" colspan="2"><font color="#006108"><strong><?=formata_numero($previsto)?></strong></font></TD>
			    
			   </TR>
	</TBODY>
	<tr><td colspan="16" style="height:1px;background-color:#e5e5e5;padding:0px"></td></tr>
	<tr><td colspan="16" style="height:2px;background-color:#000000;padding:0px"></td></tr>
	</table>
<?}?>
<br>
  <?
    $sql = "select ref.*,av.tpscod as avtps,av.corcod as avcor, av.avpid as avaid, av.tpaid as tpav, av.avptexto as avaliacao, ";
	$sql = $sql." av.avpliberada as avlib,av.usucpf as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corimgav, ca.corsignificado as avcordsc, ca.cordsc as avcornome,  tsa.tpsdsc as avtpsdsc, pa.tpscod as partps, tsp.tpsdsc as partpsdsc ,pa.corcod as parcor, ";
	$sql = $sql." pa.avpid as parid, pa.avpliberada as parlib, pa.tpaid as tppar, pa.avptexto as parecer,pa.usucpf as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata, ";
	$sql = $sql." cp.corimgpar, cp.corsignificado as pacordsc, cp.cordsc as parcornome, tsa.tpsdsc as patpsdsc, exp.exprealizado ";
	$sql = $sql." from referencia ref ";
	$sql = $sql." left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.acaid = ".$_SESSION['acaid'];
	$sql = $sql." left join cor ca on av.corcod = ca.corcod ";
	$sql = $sql." left join tiposituacao tsa on tsa.tpscod = av.tpscod ";
	$sql = $sql." left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.acaid = ".$_SESSION['acaid'];
	$sql = $sql." left join cor cp on pa.corcod = cp.corcod ";
	$sql = $sql." left join tiposituacao tsp on tsp.tpscod = pa.tpscod";
	$sql = $sql." left join execucaopto exp on ".$_SESSION['acaid']." = exp.acaid and ref.refcod = exp.refcod ";
	$sql = $sql." where ref.refdata_limite_avaliacao_aca is not null ";
	if ($_REQUEST['refcod'] and $_REQUEST['refcod'] <> 'x')
        $sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' and ref.refcod=".$_REQUEST['refcod']." order by refano_ref desc,refmes_ref desc ";
else if ($_REQUEST['refcod'])
		$sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc ";
  $RS = $db->record_set($sql);
  

  $nlinhas = $db->conta_linhas($RS);
  // com o acaid e o usucpf da sessão posso saber se o usuário terá ou não acesso à edição dos registros.
  // verifica se é coordenador de ação
  if ($db->testa_coordenador($_SESSION['acaid'],'A')) $autorizado_av = true;
  // verifica se é digitador
  if ($db->testa_digitador($_SESSION['acaid'],'A')) $autorizado_dig = true;
  // caso a autorização não tenha sido feita, testo se é super usuário
  if ($db->testa_superuser() ) {
     $autorizado_av = true;
  }
  

//}
?>
<table border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
   <?
   if ($_REQUEST['refcod'])
   {
  for ($i=0;$i<=$nlinhas;$i++)
  {
    $habil= 'N';
   $res =  $db->carrega_registro($RS,$i);
   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
   if ($avlib<>'t') $avcornome="";
   if ($parlib<>'t') $parcornome="";
?>
   <tr align="center" bgcolor="#DCDCDC"><td colspan="2" style="BORDER-TOP: #000000 2px solid; BORDER-BOTTOM: #cccccc 1px solid; color:#006699; font-size: 10pt; padding: 3px;" align="left"><img align="top" src="imagens/Sin_<?=trim($avcornome).'_'.trim($parcornome)?>.gif" border="0"> <strong>Período de Referência: <?=$refdsc?></strong> </td></tr>
   <tr><td width="50%" bgcolor="#e9e9e9" align="center" style="BORDER-RIGHT: #cccccc 1px solid;color:#990000;" valign="top">Avaliação</td><td width="50%" align="center" bgcolor="#e9e9e9" style="color:#990000;" valign="top">Acompanhamento Físico</td></tr>
   <tr>
   <td align="center" style="BORDER-RIGHT: #cccccc 1px solid;" valign="top" width="60%">
<?   
   // incia a avaliação *********************************************
    include "modulos/principal/acao/avalia_aca.inc";
   // *******************************************************************//
   //   divide avaliação de parecer
  }}
  ?>
</TR>
</tbody>
<tr><td colspan='4' class="SubTituloDireita">&nbsp;<br>
&nbsp;</td></tr>
  </table>
  <table>
<?
    if (($autorizado_av or $autorizado_dig) and $habil=='S')
    {
      if ($_REQUEST['refcod']=='x')
      {
          print '<tr><td><input type="button" class="botao" name="btgrava" value= "Gravar todas as avaliações." onclick="grava_av('.$refcod.',0,100000)">&nbsp;&nbsp;&nbsp;';
      // se for o titular e  ainda está dentro do prazo, pode liberar ou bloquear para nova edição
      print '</td></tr>';
      }
    }

    ?>
</table>
  </center>
</div>
</form>
<script>
    function verregistro(cod) {
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
   
    function grava_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='A';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      if (submited)
      {
      	alert ('Aguarde o Processamento!');
      }
      else
      {
	      document.formulario.submit();
	      var submited = true;
      }
    }
    function libera_av(refcod,cod,i)
    {

      if (document.formulario.refcod.value=='x' && cod =='0')
      {
         if( window.confirm( "Você optou por liberar as avaliações em todos os períodos.\n Caso queira liberar apenas uma avaliação, cancele sua escolha e\n opte por uma referência específica.\n\n Confirma sua escolha?") )
         {
            document.formulario.refren.value=refcod;
            document.formulario.tipograva.value='LA';
            document.formulario.doccod.value=cod;
            document.formulario.avptexto.value = i;
            document.formulario.submit();
         }
         else document.formulario.exclui.value = 0;
      } else
      {
            document.formulario.refren.value=refcod;
            document.formulario.tipograva.value='LA';
            document.formulario.doccod.value=cod;
            document.formulario.avptexto.value = i;
            document.formulario.submit();
      }
    
    
      
    }
    function bloquea_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='BA';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
	function visualiza(cod)
	{
		e = "simec.php?modulo=principal/ppafasequalitativa/acao/mostraavpar&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480'");
	}
    function envia_email(cpf)
    {
          e = "simec.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function Escolhe_referencia()
	{
      	document.formulario.submit();
 }
     function edita_dadacao()
    {
      e = "simec.php?modulo=principal/ppafasequalitativa/acao/editaracao&acao=A";
      WindowObjectReference = window.open(e, "editar_acao","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=650,height=350'");
    }

</script>
