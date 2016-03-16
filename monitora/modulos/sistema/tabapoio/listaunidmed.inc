<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listaunidmed.inc
   Finalidade: permitir o controle da lista de unidades de medida
   */

if ($_REQUEST['atualiza']==1)
{
	if ($_REQUEST['escolha']=='I') $alt='A'; else $alt='I';
	// atualiza o cadastro
    $nlinhas = count($_POST['unmcod'])-1;
    for ($j=0; $j<=$nlinhas;$j++)
    {  
  	   $sql = "update unidademedida set unmstatus='$alt' where unmcod=".$_POST['unmcod'][$j];
  	   $db->executar($sql);
    }
  $sql="update unidademedida set unmdsc=upper(unmdsc)";
     $db->executar($sql);
    $db -> commit();
    $db->sucesso($modulo);   

	
}

$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print "<form method='POST'  name='formulario'><input type ='hidden' name='atualiza'>";
if (! $_REQUEST['escolha'] or $_REQUEST['escolha']=='A') {
   $sql = "select '<input type=\"checkbox\" name=\"unmcod[]\" value=\"' || um.unmcod || '\">' as acao, um.unmdsc  as descricao from unidademedida um where um.unmstatus='A' order by descricao";
   $escolhatipo='Unidades de Medida Ativas';
}
else 
{
   $sql = "select '<input type=\"checkbox\" name=\"unmcod[]\" value=\"' || um.unmcod || '\">' as acao, um.unmdsc  as descricao from unidademedida um where um.unmstatus='I' order by descricao";
      $escolhatipo='Unidades de Medida INATIVAS';
}

$titul = 'Lista de Unidades de Medida Cadastradas no Simec';
monta_titulo($titulo_modulo,$titul);
if ($db->testa_superuser() or $db->testa_coorduma()){
?>  <p align="center">
<input type="radio" name="escolha" value='A'  onclick=fazescolha('A') <?
if (! $_REQUEST['escolha'] or $_REQUEST['escolha']=='A') print 'checked'
?>
>Ativos
<input type="radio" name="escolha" value='I' onclick=fazescolha('I') <?
if ($_REQUEST['escolha']=='I') print 'checked'
?>>Inativos
<br><b><?=$escolhatipo?>&nbsp;&nbsp;<input type="button" name="btalterar" value="Atualizar o cadastro" onclick="atualizastatus()" class="botao">
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong></strong></td>
      <td valign="top" class="title"><strong>Descrição</strong></td>            
    </tr>
</thead>
<tbody> 
<? 
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas >= 0) {
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
	print "<tr class='$marcado'><td>$acao</td><td>$descricao</td></tr>";
}
print "</table></form>";
}



}

else {
	$cabecalho = array('Ação','Descrição');
    $db->monta_lista($sql,$cabecalho,50,20,'','','');
}?>

<script>
function fazescolha(cod)
{

	document.formulario.escolha.value=cod;
	document.formulario.submit();
}
function atualizastatus()
{
	document.formulario.atualiza.value=1;
	document.formulario.submit();
}
</script>