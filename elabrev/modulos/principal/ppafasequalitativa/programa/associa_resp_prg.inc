<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:associa_resp.inc
   Finalidade: permitir a associação de responsáveis por programas
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if (! $_SESSION['prgid'])
{
  ?>
      <script>
         alert ('É necessário escolher um Programa na opção Programas - Consultar');
         lacation.href ="simec.php?modulo=principal/ppafasequalitativa/programa/listaprograma&acao=C";
      </script>
   <?
    exit();
}
if ($_REQUEST['exclui']) {
        $sql = "update usuarioresponsabilidade set rpustatus = 'I' where prgid= ".$_SESSION['prgid']." and pflcod=".$_REQUEST['exclui']." and usucpf='".$_REQUEST['resp']."'";
          $res = $db->executar($sql);
          $db->commit();
          $db->sucesso($modulo);
          
}
$sql= "select prgid, prgcod, prgdsc from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."'  and prgid = ".$_SESSION['prgid'];

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  $db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$_SESSION['prgid'] = $prgid;
$_SESSION['ident'] = $prgid;



?>
<title>Associação de Responsáveis por Programas do Simec</title>
<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center">
  <tr bgcolor="#f0f0f0">
    <td width="100%" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#D4D0C8', gradientType='1')"><label class="TituloTela">Associar Responsáveis por Programa</label></td>
  </tr>
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="procura" value=0>
<input type=hidden name="exclui" value=0>
<input type=hidden name="resp" value=0>
    <center>
    <table width='95%' align='center' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td align='right'><b>Código:</b></td>
        <td><input type="text" name="prgcod" size="5" value='<?=$prgcod?>'>
      </tr>
      <tr>
        <td align='right'><b>Descrição:</b></td>
        <td><?=campo_texto('prgdsc','N','N','',100,100,'','');?>
        </td>
      </tr>
<?
   $sql = "select pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo,rpu.usucpf, rpu.prgid, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";

    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
	        $linha = '<tr class="'.$marcado.'"><td align="right"><b>'.$descricao.':</b></td><td>'.$usuario."&nbsp;&nbsp;<img border='1' width='13' height='13' src='imagens/mais.gif'  alt='Incluir ".$descricao."' title='Incluir ".$descricao."'".' onclick="ProcuraResponsavel('.$codigo. ",'"."P'".')">';
            if ($usuario) {
              $linha = $linha.'&nbsp;&nbsp;<img border="1" width="13" height="13" src="imagens/menos.gif"  alt="Excluir "'.$descricao."' title='Excluir ".$descricao."'".' onclick="ExcluirResponsavel('.$codigo. ",'".$descricao."','".$usucpf."')".'"></td></tr>';
            }
            print $linha;
            
        }
     }
?>

  </table>
        <table>
      <tr><td><input type='button' class="botao" value='Fechar' onclick="fechar_janela()">
      </table>
  </center>
</div>
</form>
<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
    function ProcuraResponsavel(cod,Origem)
    {
       if (document.formulario.prgcod.value != '')
       {
          e = "simec.php?modulo=sistema/perfil/associa_resp&acao=A&cod="+cod+"&ogm="+Origem;
          WindowObjectReference = window.open(e, "Relação_Gerentes_Programa",
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
  

</script>

