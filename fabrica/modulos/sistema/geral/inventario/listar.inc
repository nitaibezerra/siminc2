<?php
header( 'content-type: text/html; charset=iso-8859-1;' );
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

$perfis = arrayPerfil();

//Monta o Titulo da página abaixo da Aba
monta_titulo( "INVENTÁRIO", "<b>Consultar Inventário</b>" );

$sidid  = (int) $_REQUEST['sidid'];
$where  = '';
if( !empty($sidid) ) {
    $where = "AND inv.sidid = {$sidid}";
}

$acoes = '';

if( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_FISCAL_CONTRATO, $perfis )){
    $linkEditar   = "<a href=\"fabrica.php?modulo=sistema/geral/inventario/alterarInventario&acao=A&co_inventario='|| inv.co_inventario ||'\">";
    $linkEditar  .= "<img style=\"border: none;\" src=\"/imagens/editar_nome.gif\" alt=\"Editar\" title=\"Editar\"/></a>";
    $linkExcluir  = "<a onclick=\"excluirInventario(\''|| inv.co_inventario ||'\')\">"  ;
    $linkExcluir .= "<img style=\"border: none;cursor:pointer;\" src=\"/imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\"/></a>";

    $acoes  = $linkEditar . $linkExcluir;
}

$sql = "SELECT 
    CASE  WHEN usucpfencerramento IS NULL OR dt_encerramento IS NULL THEN '{$acoes}'
          ELSE '<a href=\"fabrica.php?modulo=sistema/geral/inventario/visualizarInventario&acao=A&co_inventario='|| inv.co_inventario ||'\">
                <img style=\"border: none;\" src=\"/imagens/consultar.gif\" alt=\"Consultar\" title=\"Visualizar\"/></a>'
		   END as acoes, 
				  sist.siddescricao, 
				  '<span>'|| COALESCE (SUM (qtd_pf),0) ||'</span>' AS qtd_pf,
	 CASE WHEN dt_encerramento is null OR usucpfencerramento is null THEN 'Em Desenvolvimento' 
          ELSE 'Finalizada'		
           END AS situacao
		  FROM inventario.tb_simec_inventario AS inv
    INNER JOIN demandas.sistemadetalhe AS sist 
            ON inv.sidid = sist.sidid
          LEFT JOIN inventario.tb_simec_funcionalidade as func
            ON inv.co_inventario = func.co_inventario
         WHERE inv.st_inventario = 'A'
      {$where}
	  GROUP BY inv.co_inventario, sist.siddescricao, dt_encerramento, usucpfencerramento, dt_encerramento";

$sistemaRepositorio = new SistemaRepositorio();
$sistemasAtivos = $sistemaRepositorio->recuperarInventariados();

?>

<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>

<form id="form-listaInventario" method="post" action="?modulo=sistema/geral/inventario/listar&acao=A">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Sistema:</td>
			<td><select id="sidid" class="campoEstilo" name="sidid" style="width: auto;">
					<option value="">Selecione</option>
					<?php foreach ( $sistemasAtivos as $sa ) { ?>
						<option value="<?php echo $sa->getId(); ?>" 
							<?php echo $sa->getId() == $sidid ? "selected=selected" : "" ;?>><?php echo $sa->getDescricao(); ?></option>
					<?php } ?>
				</select>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro" align="center"><input id="submit-listaAuditoriaGc" type="submit" value="Consultar" /></td>
		</tr>
	</table>
</form>
<?php
	$cabecalho		= array('Ações','Sistema','QTD PF','Situação');
	$tamanho		= array("10%","50%","20%","20%");
	$alinhamento	= array("center","center","center","center");
	$db->monta_lista( $sql, $cabecalho, 20, 5, 'N', 'center', $par2, "", $tamanho, $alinhamento );
?>

<?php if( in_array(PERFIL_FISCAL_CONTRATO, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( PERFIL_CONTAGEM_PF, $perfis ) ) { ?>
    
    <table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" class="alignRight">
                <input type="button" value="Inserir Novo" onclick="parent.location='fabrica.php?modulo=sistema/geral/inventario/inserirInventario&acao=A'"/>
            </td>
        </tr>
    </table>
<?php }?>
<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script type="text/javascript" language="javascript">

function excluirInventario(coInventario)
    {
        var params  = { 'co_inventario' : coInventario };
        
        if( confirm( 'Confirma a exclusão do inventário?' ) ){
            
           $.ajax({
               beforeSend: function(){
                $("#dialogAjax").show();
               },
               type: 'post',
               url:'geral/inventario/remover_inventario.php',
               dataType: 'json',
               data: params,
               success: function(response){
                   if( response.status == true )
                   {
                       window.location.reload();
                   }
                },
                complete: function(){
                    $("#dialogAjax").hide();
                }
            });
        }
    }

</script>
