<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:respprg.inc
   Finalidade: permitir a construção de relatorio de responsabilidades de programas
    */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['relat']=='r')
{
  // monta relatorio
  $i=0;
  $sql = "select distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"alterar_cad(' || ' \'' || p.prgid || '\' ' || ')\">' as acao, p.prgcod as codigo ,p.prgdsc as descricao from programa p where  p.prgstatus='A' and p.prgano='".$_SESSION['exercicio']."' and p.prgid  not in (select distinct prgid from usuarioresponsabilidade where (prgid is not null and rpustatus='A') and (";

  // atenção !!!! na sql acima, o espaço é importante para não causar erro!!!!

  $titul = 'Você buscou os Programas que não tenham: ';
  $tit = 'Relatório de programas sem';
  
  
  foreach($_POST as $k=>$v) {
     if (strpos($k,'fl') > 0) {
         $i++;
         if ($i == 1) $sql = $sql." pflcod=".$v;
         else $sql = $sql."   or pflcod=".$v;
         $sqldsc = 'select pfldsc from perfil where pflcod='.$v;
         $titulo=$db->recuperar($sqldsc);
         $titul = $titul.$titulo['pfldsc'].' ';
         $tit = $tit.': '.$titulo['pfldsc'];
     }
  }
  $sqlrel = $sql." ))";
  if ($i == 0) unset($sqlrel);
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Relatório de Responsabilidades em Programas';
monta_titulo($titulo_modulo,'');
?>
<div align="center">
<center>



<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="relat" >

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
<tr><td colspan=2 align='center'><b> Programas sem:</b></td></tr>
<?
  $sql = "select distinct pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo  from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod  and rpu.rpustatus='A' where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') and pfl.sisid=".$_SESSION['sisid']." order by pfl.pfldsc";
  

  
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td class='SubTituloDireita' ><b>".$descricao."</b></td><td><input type='checkbox' name='pfl".$codigo."' value='".$codigo."'></td></tr>";
            print $linha;

        }
     }
     ?>

<tr bgcolor="#CCCCCC">
   <td></td>   <td align='center'><input type="button" name="btrelat" value="Gerar Relatório" onclick="validar_relatorio()" class="botao"></td>

</tr>
</table>
</center>
</div>
</form>
<?

if ($sqlrel){
    $RS = $db->record_set($sqlrel);
    $_SESSION['LINHA'] = $db->conta_linhas($RS);
  $_SESSION['title'] = $tit;
  $_SESSION['TIT'][0][0] = 'Código ';
  $_SESSION['TIT'][0][1] = 'Título';
  $_SESSION['data']= date('(h:m) d/m/Y');
  $_SESSION['LIN'] = array();
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0) print '<input type="button" class="botao" name="btrelat" value ="Gerar PDF" align="left" onclick="imprime_rel()">';
    print '</td></tr></table>';
    $cabecalho = array('Ação','Código ','Título');
    $db->monta_lista($sqlrel,$cabecalho,100,20,'','','');
    //
    if ($_SESSION['LINHA'] >= 0) {
        for ($i=0; $i<=$_SESSION['LINHA'];$i++){
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
 	       $_SESSION['LIN'][$i][0] = $codigo;
		   $_SESSION['LIN'][$i][1] = $descricao;
        }
     }

}
?>

<script>
function validar_relatorio()
{
    document.formulario.relat.value='r';
   	document.formulario.submit();
}
function alterar_cad(cod)
{
   	location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadprograma&acao=C&prgid="+cod;

}

function imprime_rel()
{
				     //Abre popup em branco
   	 janela = window.open('about:blank',"relatorio","menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
   	   //Abre relatório pdf no popup
   	janela.location = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";


}

</script>
