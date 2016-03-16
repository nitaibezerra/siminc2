<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listunidade.inc
   Finalidade: permitir o controle da lista de unidades
   */

$modulo=$_REQUEST['modulo'] ;//

include APPRAIZ."includes/cabecalho.inc";

if ($_POST['exclui'])
{
    $sql = "update unidade set unistatus='".$_REQUEST['stat']."' where unicod='".md5_decrypt($_POST['exclui'],'')."'	";
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo,'&orgcod='.$_REQUEST['orgcod2']);
}


if ($_POST['act']=='valalterar')
{
    $sql = "update unidade set unidsc='".$_REQUEST['unidsc']."', uniabrev='".$_REQUEST['uniabrev']."' where unicod='".md5_decrypt($_POST['uni'],'')."'";

 
       $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo,'&orgcod='.$_REQUEST['orgcod2']);
}
$titul = 'Lista de Unidades cadastradas';

$sql= "select u.unidsc, u.uniabrev from unidade u where unicod='".md5_decrypt($_POST['uni'],'')."'";
$res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if ($_REQUEST['orgcod2']) $orgcod=$_REQUEST['orgcod2'];
if ($_REQUEST['orgcod']) $orgcod=$_REQUEST['orgcod'];
?>
<br>
<?
if ($_REQUEST['acao']== 'A') $titulo_modulo='Consultar Unidades';
if ($_REQUEST['acao']== 'C') $titulo_modulo='Consultar Unidades';
monta_titulo($titulo_modulo,'');

?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<form method="POST" name="formulario">
	<input type=hidden name="modulo" value="<?=$modulo?>">
<input type='hidden' name='exclui' value=0>
<input type='hidden' name='act' value=<?=$_REQUEST['act']?>>	
<input type='hidden' name='uni' value=<?=$_REQUEST['uni']?>>
<input type='hidden' name='orgcod2' value=<?=$orgcod?>>
<input type='hidden' name='stat' >

    <tr>
        <td align='right' width="20%" class="SubTituloDireita">Orgão:</td>
        <td >
	<?$sql2 = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao where organo='".$_SESSION['exercicio']."'  order by orgdsc ";
	  $db->monta_combo("orgcod",$sql2,$habil,"Selecione o órgão",'atualizaComboUnidade','');
	?><?=obrigatorio();?></td>
	
      </tr>
		<tr>
			<td class="SubTituloDireita" colspan="2">&nbsp;</td>
		</tr>
      </table>
  <? if ($_REQUEST['act']=='alterar')
  {
  	?>
  
  	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
	<td>
		<?=campo_textarea('unidsc','S','S','',100,3,'','');?>
	</td>
       <tr>
            <td align='right' class="SubTituloDireita">Abreviatura:</td>

           <td>
		<?=campo_textarea('uniabrev','S','S','',100,1,'','');?>
	   </td>
      </tr>
      </tr>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   <input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
 </tr>      
      </table>
      <?
  }
 ?>  
  
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Status</strong></td>      
      <td valign="top" class="title"><strong>Código</strong></td>     
      <td valign="top" class="title"><strong>Nome</strong></td>
      <td valign="top" class="title"><strong>Abreviatura</strong></td>
      <td valign="top" class="title"><strong>Órgão Superior</strong></td>
    </tr>
</thead>
<tbody> 
<?   
   $sql= "select distinct case when u.unistatus='A' then 'Ativo' else 'Inativo' end as unistatus,case when u.unistatus='A' then 'I' else 'A' end as status,u.unicod as codigo, u.unidsc as descricao, u.uniabrev as abreviatura, o.orgdsc from unidade u left join orgao o on u.orgcod = o.orgcod";
if ($_REQUEST['orgcod'] ) {
    $sql = $sql . "  where u.orgcod ='".$_REQUEST['orgcod']."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
}
$sql = $sql. "  order by status desc,descricao";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
 if ($_REQUEST['orgcod'] ) $titul .= ' Subordinadas a(o) '.$orgdsc;
 
 
 if ($nlinhas >= 0) {
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
	if ($db->testa_superuser() )
  print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Alterar unidade." onclick="altera_unidade('."'".md5_encrypt($codigo,'')."'".')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Tornar Inativa/Ativa no sistema." onclick="excluir_unidade('."'".md5_encrypt($codigo,'')."','".$status."','".$descricao."'".')"></td><td>'.$unistatus.'</td><td>'.$codigo.'</td><td>'.$descricao.'</td><td>'.$abreviatura.'</td><td>'.$orgdsc.'</td></tr>';
  else 
  print '<tr class="'.$marcado.'"><td>Sem ação possível</td><td>'.$unistatus.'</td><td>'.$codigo.'</td><td>'.$descricao.'</td><td>'.$orgdsc.'</td></tr>';
  
} }
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de perfil</td></tr>';
}
?>
</tbody>
    </table>
    </center>
  </div>
</form>


<script>
  function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }
    
  function altera_unidade(cod) {
    document.formulario.act.value = 'alterar';
    document.formulario.uni.value = cod;    
     document.formulario.submit();
  }
  
  function validar_cadastro(cod) 
{
	    document.formulario.act.value = 'valalterar';
     document.formulario.submit();
}
  function excluir_unidade(cod,stt,dsc) {
  
    if( window.confirm( "Confirma a exclusão da Unidade "+dsc+" ?") )
    {
	document.formulario.exclui.value = cod;
    document.formulario.stat.value = stt;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
    
  }
    </script>