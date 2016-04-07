<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral, Gilberto Arruda Cerqueira Xavier
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listaprograma.inc
   Finalidade: permitir o controle da lista de programas
   */
//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
$wh = "";

//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?
$tipoprgaca = "Programas";
	$sql = "select p.prgid, p.prgcod as codigo, prgdsc as descricao, p.prsano, count(*) as numacoes from elabrev.ppaprograma_proposta p where p.prgstatus in ('A','N') and p.orgcod='".$_SESSION['ittorgao']."' and p.prsano = '".$_SESSION['exercicio']."'  ".$wh." group by p.prgid, p.prgcod , p.prgdsc, p.prsano order by prgcod";
	$subtit1 = 'de Programas'; 
	$subtit2 = 'do Programa';


$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Elaboração e Revisão de Programas','Clique no código entrar da elaboração e revisão - Total '.$subtit1.' ('.($nlinhas+1).')');?>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" width="10%">
         <strong>Código</strong></td>
      <td valign="top"  width="80%"><strong>
      Título do Programa</strong></td>
    </tr>
</thead>
<tbody>
<?

for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
?>
     <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
		 <td><a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/programa/cadprograma&acao=C&prgid=<?=$prgid?>"><?=$codigo?></a></td>
		 <td ><?=$descricao?></td>
	 </tr>
<?}
?>
</tbody>
</table>

      </td>
    </tr>
  </table>
  </center>
</div>
<script language="JavaScript">
function filtra()
{
document.formulario.submit();
}

function ordena(ordem, direcao)
{
document.formulario.ordemlista.value=ordem;
document.formulario.ordemlistadir.value=direcao;
document.formulario.submit();
}
</script>
