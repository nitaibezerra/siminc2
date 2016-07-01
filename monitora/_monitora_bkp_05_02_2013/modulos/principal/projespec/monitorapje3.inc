<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitoraacao.inc
   finalidade: permitir a avaliação do projeto especial
   */
$modulo=$_REQUEST['modulo'];
if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

if ($_REQUEST['tipograva']== 'A' and $_REQUEST['avptexto']=='100000')
{
  $erro_total=0;
  foreach($_POST as $k=>$v)
  {
    if (substr($k,0,10)=='avaliacao_')
     {
        $erro = 0;
        ${$k}=$v;
        dbg($parte);
        $parte = substr($k,10,strlen($k)-10);
        $refer = substr($parte,0,strpos($parte,'_'));
        $parte2 = substr($parte,strpos($parte,'_')+1,100);
        $codigo = substr($parte2,0,strpos($parte2,'_'));
        $ordem = substr($parte2,strpos($parte2,'_')+1,100);

        $texto = str_replace("'","''",$texto);
        
        $cor =  $_REQUEST['corcodav'.$ordem];
	    $sit =  $_REQUEST['tpscodav'.$ordem];
	    $exp =  $_REQUEST['exprealizado'.$ordem];
	    $expfin = $_REQUEST['expfinanceiro'.$ordem];

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
               $sql= "insert into monitora.avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,pjeid,avptexto,avpstatus,avpdata) values (1,1,".$refer.",'".$_SESSION['usucpf']."',".$_POST['corcodav'.$ordem].",".$_POST['tpscodav'.$ordem].",".$_SESSION['pjeid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
           else
               $sql= "update monitora.avaliacaoparecer set refcod=".$refer.",usucpf='".$_SESSION['usucpf']."',corcod=".$_POST['corcodav'.$ordem].",tpscod=".$_POST['tpscodav'.$ordem].",pjeid=".$_SESSION['pjeid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$codigo;
           $db->executar($sql);
           if ($_POST['exprealizado'.$ordem] <>'-' or $_POST['expfinanceiro'.$ordem] <>'-' )
           {
			  $sql = "select * from monitora.execucaopto where refcod=".$refer." and pjeid=".$_SESSION['pjeid'];
			  
		     if($db->eof($sql))
		     {
			    $sql = "insert into monitora.execucaopto (expfinanceiro,exprealizado, usucpf, pjeid, refcod) values (".$_POST['expfinanceiro'.$ordem].",".$_POST['exprealizado'.$ordem].",'".$_SESSION['usucpf']."',".$_SESSION['pjeid'].",".$refer.")";
		     }
             else
		     {
       		     $sql = "update monitora.execucaopto set exprealizado=".$_POST['exprealizado'.$ordem].", expfinanceiro=".$_POST['expfinanceiro'.$ordem]." ,usucpf='".$_SESSION['usucpf']."', expdata='".date('Y/m/d')."' where pjeid=".$_SESSION['pjeid']." and refcod=".$refer;
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
    $expfin =  $_REQUEST['expfinanceiro'.$_REQUEST['avptexto']];	
	
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
	   if ($exp == '') {
	   ?>
	      <script>
	         alert ('Informe o Executado Físico no Período. Campo Obrigatório');
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
		       $sql= "insert into monitora.avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,pjeid,avptexto,avpstatus,avpdata) values (1,1,".$_REQUEST['refren'].",'".$_SESSION['usucpf']."',".$cor.",".$sit.",".$_SESSION['pjeid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
			   //print $sql.'<br>';
			   $db->executar($sql);
		     }
		     else
		     {
		       $sql= "update monitora.avaliacaoparecer set refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",pjeid=".$_SESSION['pjeid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
			   //print $sql.'<br>';
			   $db->executar($sql);
		     }
		}
		if ($_REQUEST['tipograva']== 'LA')
			{
      $sql= "update monitora.avaliacaoparecer set avpliberada='t',refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",pjeid=".$_SESSION['pjeid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
	 $db->executar($sql);
			}
		//Grava o executado
		if ($exp<>'-' or $expfin<>'')
		{
			 $sql = "select * from monitora.execucaopto where refcod=".$_REQUEST['refren']." and pjeid=".$_SESSION['pjeid'];
			 
			 if ($exp=='-') $exp=0;
			 if ($expfin=='-') $expfin=0;
			 if($db->eof($sql))
			{
				$sql = "insert into monitora.execucaopto (exprealizado, usucpf, pjeid, refcod) values (".$exp.",'".$_SESSION['usucpf']."',".$_SESSION['pjeid'].",".$_REQUEST['refren'].")";
				
			}else
			{
				$sql = "update monitora.execucaopto set exprealizado=".$exp.", usucpf='".$_SESSION['usucpf']."', expdata='".date('Y/m/d')."' where pjeid=".$_SESSION['pjeid']." and refcod=".$_REQUEST['refren'];
				
			}
			//print $sql;
			
			$db->executar($sql);
		}


	 $db->commit();
	 $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	 exit();
}
if ($_REQUEST['tipograva']== 'BA')
{
     $sql= "update monitora.avaliacaoparecer set avpliberada='f' where avpid=".$_REQUEST['doccod'];
	 $db->executar($sql);
	 $db->commit();
	 $db->sucesso($modulo, '&refcod='.$_REQUEST['refcod']);
	 exit();
}

include  APPRAIZ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Avaliação de Projeto Especial';
monta_titulo($titulo_modulo,'');
$sql= "select p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod where p.pjeid=".$_SESSION['pjeid'];

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  $db->carrega_registro($RS,0);
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if (! $_REQUEST['refcod'])
{
  $sql = "select refcod from monitora.referencia where refsnmonitoramento='t' and refdata_inicio < now()::date and refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc";
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
<script language="javascript" type="text/javascript" src="../includes/tiny_mce_gzip.php"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, print,preview,fullscreen,separator",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw",
	width : "100%"
	});
</script>
<!--<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">-->
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$pjecod.'.'.$pjedsc?></td>
      </tr>
<?
   $sql = "select distinct pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || ' ('|| org.orgdsc ||')' as usuario from perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$_SESSION['pjeid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pfldsc";
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
$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from monitora.referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref desc";
$db->monta_combo("refcod",$sql2,'S',"Escolha o período de referência",'Escolhe_referencia()','Todos os Períodos de Referência');
?>
</td></tr>
  </table>
  <?
    $sql = "select ref.*,av.tpscod as avtps,av.corcod as avcor, av.avpid as avaid, av.tpaid as tpav, av.avptexto as avaliacao, ";
	$sql = $sql." av.avpliberada as avlib,av.usucpf as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corimgav, ca.corsignificado as avcordsc, ca.cordsc as avcornome,  tsa.tpsdsc as avtpsdsc, pa.tpscod as partps, tsp.tpsdsc as partpsdsc ,pa.corcod as parcor, ";
	$sql = $sql." pa.avpid as parid, pa.avpliberada as parlib, pa.tpaid as tppar, pa.avptexto as parecer,pa.usucpf as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata, ";
	$sql = $sql." cp.corimgpar, cp.corsignificado as pacordsc, cp.cordsc as parcornome, tsa.tpsdsc as patpsdsc, exp.exprealizado, exp.expfinanceiro ";
	$sql = $sql." from monitora.referencia ref ";
	$sql = $sql." left join monitora.avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.pjeid = ".$_SESSION['pjeid'];
	$sql = $sql." left join cor ca on av.corcod = ca.corcod ";
	$sql = $sql." left join tiposituacao tsa on tsa.tpscod = av.tpscod ";
	$sql = $sql." left join monitora.avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.pjeid = ".$_SESSION['pjeid'];
	$sql = $sql." left join cor cp on pa.corcod = cp.corcod ";
	$sql = $sql." left join tiposituacao tsp on tsp.tpscod = pa.tpscod";
	$sql = $sql." left join monitora.execucaopto exp on ".$_SESSION['pjeid']." = exp.pjeid and ref.refcod = exp.refcod ";
	$sql = $sql." where ref.refdata_limite_avaliacao_aca is not null ";
	if ($_REQUEST['refcod'] and $_REQUEST['refcod'] <> 'x')
        $sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' and ref.refcod=".$_REQUEST['refcod']." order by refano_ref desc,refmes_ref desc ";
else if ($_REQUEST['refcod'])
		$sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc ";
		
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  // com o pjeid e o usucpf da sessão posso saber se o usuário terá ou não acesso à edição dos registros.
  // verifica se é coordenador de ação
  if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) $autorizado_av = true;
  // verifica se é digitador
  if ($db->testa_digitador($_SESSION['pjeid'],'E')) $autorizado_dig = true;
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
   <tr align="center" bgcolor="#DCDCDC"><td colspan="2" style="BORDER-TOP: #000000 2px solid; BORDER-BOTTOM: #cccccc 1px solid; color:#006699; font-size: 10pt; padding: 3px;" align="left"><img align="top" src="../imagens/Sin_<?=trim($avcornome).'_'.trim($parcornome)?>.gif" border="0"> <strong>Período de Referência: <?=$refdsc?></strong> </td></tr>
   <tr><td width="50%" bgcolor="#e9e9e9" align="center" style="BORDER-RIGHT: #cccccc 1px solid;color:#990000;" valign="top">Avaliação (textual)</td><td width="50%" align="center" bgcolor="#e9e9e9" style="color:#990000;" valign="top">Avaliação (parâmetros)</td></tr>
   <tr>
   <td align="center" style="BORDER-RIGHT: #cccccc 1px solid;" valign="top" width="60%">
<?   
   // incia a avaliação *********************************************
    include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/projespec/avalia_pje.inc");
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
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
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
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/mostraavpar&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480'");
	}
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function Escolhe_referencia()
	{
      	document.formulario.submit();
 }


</script>
