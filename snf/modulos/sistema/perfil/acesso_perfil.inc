 <?
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Marcelo Freire Costa (marcelofreire.freire@gmail.com)
   Programador: Marcelo Freire Costa (e-mail: marcelofreire.freire@gmail.com)
   Módulo:associa_perfil.inc
   Finalidade: permitir a associação de um perfil as operações de menu
   */
   
//Armazena o caminho de redirecionamento
$modulo=$_REQUEST['modulo'] ;

//Caso seja passado o código do perfil atribui-se a variável $pflcod
if ($_REQUEST['pflcod']) $pflcod = $_REQUEST['pflcod'];

//Inclui o cabeçalho da página
include APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?
//cria as abas
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<?
//título da página
monta_titulo($titulo_modulo,'Selecione os Módulos para este Perfil.');
?>

<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Associação do Perfil:</td>
		<td>
			<?
			  $sisid = $_SESSION['sisid'];
			 //Pesquisa os perfis cadastrados para o sistema para montar a combo
			  $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil where pflstatus='A' and sisid=".$_SESSION['sisid']." order by pfldsc ";
			  //monta a combo de perfis
			  $db->monta_combo("pflcod",$sql,'S',"Selecione o Perfil",'submete_perfil','');
			?>
		</td>
	</tr>
</table>
<?
if ($pflcod && $sisid) {
//teste utilizando a função Monta Lista
$cabecalho = array('Código','Menu / Módulo','Visível','Link','Transação');

$sql= "SELECT  
			menu.mnucod, 
			case 
				when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
				when  mnutipo=3 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
				when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc 
				else mnudsc 
			end as mnudsc, 
			case 
				when mnushow=false then '<font color=#808080>Não</font>' 
				else '<font color=#008000>Sim</font>' 
			end as mnushow, 
			mnulink, 
			mnutransacao 
		FROM 
			menu 
		INNER JOIN perfilmenu p on menu.mnuid = p.mnuid AND p.pflcod=".$pflcod." 
		WHERE  
			mnustatus = 'A' 
			AND menu.sisid=".$sisid."  
		ORDER BY 
			mnucod";

//Monta a lista de menus do perfil
$db->monta_lista_simples($sql,$cabecalho,300,20,'','','');
}
?>
</form>
<script language="JavaScript">
  function submete_perfil(cod){
  	//Caso o código do perfil e do sistema sejam informados
  	    if (document.formulario.pflcod.value != '' || document.formulario.sisid.value != '')
    {
    	//Submete a página
        document.formulario.submit();
    }
  }
</script>