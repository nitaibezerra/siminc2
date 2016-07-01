<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:assocrespac.inc
   Finalidade: permitir a associação de responsáveis por ação
   */
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

if (! $_SESSION['acaid'])
{
	   ?>
	      <script>
	         alert ('É necessário escolher uma Ação na opção Ações - Consultar');
   history.back();
	      </script>
	   <?
	     exit();
}
if ($_REQUEST['exclui'])
{
        //$sql = "update usuarioresponsabilidade set rpustatus = 'I' where acaid= ".$_SESSION['acaid']." and pflcod=".$_REQUEST['exclui']." and usucpf='".$_REQUEST['resp']."'";
        $sql="select acaid from elabrev.ppaacao where acacod='".$_SESSION['acacod']."'";
        $RS = $db->record_set($sql);
        $nlinhas = $db->conta_linhas($RS);
        for ($i=0;$i<=$nlinhas;$i++)
        {
        	$res =  @$db->carrega_registro($RS,$i);
        	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        	$sql = "delete from  ".$_SESSION['sisdiretorio'].".usuarioresponsabilidade where acaid= ".$acaid." and pflcod=".$_REQUEST['exclui']." and usucpf='".$_REQUEST['resp']."'";
          $res = $db->executar($sql);
        }
        $db->commit();
        $db->sucesso($modulo);
}

/////////////////////////
$sql= "select a.prgcod, p.prgdsc, a.prgid, a.acaid, a.acacod, a.acadsc from elabrev.ppaacao_proposta a, elabrev.ppaprograma_proposta p where a.prgid=p.prgid and a.acastatus='A' and a.prsano = '".$_SESSION['exercicio']."'  and a.acaid =".$_SESSION['acaid']." limit 1";

$RS = $db->record_set($sql);

$nlinhas = $db->conta_linhas($RS);
$res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$acacod1 = $acacod;
$acadsc1 = $acadsc;

$_SESSION['acaid'] = $acaid;
$_SESSION['ident'] = $acaid;

$sql= "select distinct * from ".$_SESSION['sisdiretorio'].".usuarioresponsabilidade u, acao b where u.rpustatus='A' and b.prgano = '".$_SESSION['exercicio']."'  and u.acaid=b.acaid and u.acaid = ".$_SESSION['acaid'];

$RS2 = $db->record_set($sql);
$nlinhas2 = $db->conta_linhas($RS2);

?>
<title>Associação de Responsáveis por Ação do Simec</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<div align="center">
<table  width='95%' border="0" cellspacing="0" cellpadding="3" align="center">
  <tr bgcolor="#f0f0f0">
    <td width="100%" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#D4D0C8', gradientType='1')"><label class="TituloTela">Associar Responsáveis por Ação</label></td></tr>
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="procura" value=0>
<input type=hidden name="exclui" value=0>
<input type=hidden name="resp" value=0>
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <td align='right' class="SubTituloDireita"><b>Programa:</b></td>
    <td><?=campo_texto('prgcod','N','','',5,5,'','');?><?=campo_texto('prgdsc','N','','',50,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>Código:</b></td>
    <td><?=campo_texto('acacod1','N','','',5,5,'','');?>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>Título:</b></td>
    <td><?=campo_texto('acadsc1','N','N','',80,80,'','');?></td>
  </tr>
<?

   $sql = "select pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo,rpu.usucpf, rpu.acaid, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join elabrev.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.acaid = ".$acaid." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('A','X','D') and pfl.sisid=".$_SESSION['sisid']." order by pfl.pfldsc";


    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
	        $linha = '<tr class="'.$marcado.'"><td align="right" class="SubTituloDireita"><b>'.$descricao.':</b></td><td>'.$usuario."&nbsp;&nbsp;<img border='1' width='13' height='13' src='../imagens/mais.gif'  alt='Incluir ".$descricao."' title='Incluir ".$descricao."'".' onclick="ProcuraResponsavel('.$codigo. ",'"."A'".')">';
            if ($usuario) {
              $linha = $linha.'&nbsp;&nbsp;<img border="1" width="13" height="13" src="../imagens/menos.gif"  alt="Excluir "'.$descricao."' title='Excluir ".$descricao."'".' onclick="ExcluirResponsavel('.$codigo. ",'".$descricao."','".$usucpf."')".'"></td></tr>';
            }
            print $linha;
            
        }
     }
?>

  </table>
  </center>
</form>
<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
    function ProcuraResponsavel(cod,Origem)
    {
       if (document.formulario.prgcod.value != '')
       {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/perfil/associa_resp&acao=A&cod="+cod+"&ogm="+Origem;
          WindowObjectReference = window.open(e, "Relação_dos_Responsáveis",
"menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=600,height=400'");
        }
    }
  function fechar_janela()
  {
    window.close();
        opener.location.reload();
  }

  function ExcluirResponsavel(cod,dsc,usu) {

    if( window.confirm( "Confirma a exclusão da responsabilidade de "+dsc+" ?") )
    {
	   document.formulario.exclui.value = cod;
	   document.formulario.resp.value = usu;
	   document.formulario.submit();
    } else document.formulario.exclui.value = 0;

  }
  
    function ProcuraPrograma(form) {
    	document.formulario.procura.value = document.formulario.prgcod.value;
    	document.formulario.submit();
    }


ok=false;

function CheckAll()
{
	if(!ok)
    {
	  for (var i=0;i<document.formulario.elements.length;i++)
      {
        //document.formulario.descricao.value.substring(val1,val2);
	    var x = document.formulario.elements[i];
	    if (x.name.substring(0,2) == 'CB')
        {
		    	x.checked = true;
				ok=true;
		}
	  }
	}
	else
    {
	   for (var i=0;i<document.formulario.elements.length;i++)
        {
	       var x = document.formulario.elements[i];
	       if (x.name.substring(0,2) == 'CB')
            {
		    	x.checked = false;
				ok=false;
            }
         }
    }
}

function efetua(opt)
{
  var str='';
  var tot=0;
  for (var i=0;i<document.formulario.elements.length;i++)
  {
     var CB = document.formulario.elements[i];
     if (CB.name.substring(0,2) == 'CB')
     {
        if (CB.checked)
	    {
           str = str+CB.value+',';
           tot++;
        }
     }
  }
  if (opt == 'rep')
  {
     if (str == '')
     {
       alert("Selecione os registros a serem replicados e clique aqui novamente");
       return false ;
     }
     else
     {
       var msg='';
       if (tot==1)
       {
          msg = "Deseja realmente replicar o registro selecionado?";
       }
       else
       {
          msg = "Deseja realmente replicar os "+tot+" registros selecionados?";
       }
       if (confirm(msg))
       {
          for (var i=0;i<document.formulario.elements.length;i++)
          {
	     var w = document.formulario.elements[i];
	     if (w.name.substring(0,2) == 'CB')
             {
                if  (w.checked)
                {
                  if (document.formulario.lista.value != '')
                  {
		   document.formulario.lista.value = document.formulario.lista.value + "," + w.name.substring(2,8);
		          }
		          else
		          {
                        document.formulario.lista.value = w.name.substring(2,8);
                  }
                }
             }
         }

         document.formulario.submit();
       }
       else
       {
	       return false;
       }
     }
  }
}
</script>

