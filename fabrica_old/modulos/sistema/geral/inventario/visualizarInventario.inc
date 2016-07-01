<?php
header( 'content-type: text/html; charset=iso-8859-1;' );
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";


//Monta o Titulo da página abaixo da Aba
monta_titulo( "INVENTÁRIO", "<b>Visualizar Inventário</b>" );

$coInventario = $_REQUEST['co_inventario'];
if(empty($coInventario)){
?>   
    <script type="text/javascript">
        alert('Erro a acessar a página, por favor acesse novamente');
        history.go(-1);
    </script>
<?php    
    exit;
}

$sql = "SELECT agrup.no_agrupador AS Agrupador, 
               ds_funcionalidade AS Funcionalidade, 
               ds_tipo_funcionalidade AS Tipo, 
			   '<a style=\"color:#0066cc;\" href=\"#\" onclick=\"abreDescricao(\''|| ds_alr_rlr ||'\', \'Descrição ALR/RLR\')\" >'|| qt_alr_rlr ||'</a>' AS Qtd_LR, 
               '<a style=\"color:#0066cc;\" href=\"#\" onclick=\"abreDescricao(\''|| ds_td ||'\', \'Descrição TD\')\" >'|| qt_td ||'</a>' AS Qtd_TD,
     CASE WHEN tp_complexidade = 'A' THEN 'Alta'
          WHEN tp_complexidade = 'M' THEN 'Média'
          WHEN tp_complexidade = 'B' THEN 'Baixa'
        END AS tp_complexidade,
        	   '<span>'||func.qtd_pf ||'</span>' AS QTD_PF
          FROM inventario.tb_simec_funcionalidade AS func
    INNER JOIN inventario.tb_simec_agrupador_funcionalidade AS agrup
            ON func.co_agrupador = agrup.co_agrupador
    INNER JOIN inventario.tb_simec_tipo_funcionalidade AS tipo
            ON func.co_tipo_funcionalidade = tipo.co_tipo_funcionalidade
     	 WHERE func.co_inventario = {$coInventario}";

     
$sqlInventario  = "SELECT inv.co_inventario, 
						  to_char(inv.dt_cadastro,'DD/MM/YYYY') as dt_cadastro, 
						  sid.sidid,
                    	  sid.siddescricao, 
                    	  usu.usunome, usu.usucpf,
                    	  SUM( func.qtd_pf ) as total_pf
          			 FROM inventario.tb_simec_inventario inv
          	   INNER JOIN demandas.sistemadetalhe sid
            		   ON inv.sidid = sid.sidid   
          	   INNER JOIN seguranca.usuario usu
            		   ON  inv.usucpf = usu.usucpf
          		LEFT JOIN inventario.tb_simec_funcionalidade func
            		   ON inv.co_inventario = func.co_inventario
     				WHERE inv.co_inventario = {$coInventario}
     			 GROUP BY inv.co_inventario, 
     			 		  inv.dt_cadastro, 
     			 		  sid.sidid,
                    	  sid.siddescricao, 
     			 		  usu.usunome, 
     			 		  usu.usucpf";
     
$dadosInventario = $db->pegaLinha($sqlInventario);

$sistema                = $dadosInventario['siddescricao'];
$dataCriacao            = $dadosInventario['dt_cadastro'];
$cadastroRealizadoPor   = $dadosInventario['usunome'];
$totalPf                = $dadosInventario['total_pf'];

?>

<link rel='stylesheet' type='text/css' href='./css/jquery-ui-1.8.16.custom.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>

<script type="text/javascript">

function btnVoltar(){
	window.location = 'fabrica.php?modulo=sistema/geral/inventario/listar&acao=A';
}

</script>

<form id="form-listaAuditoriaGc" method="post" action="?modulo=principal/gerencia-configuracao/listar&acao=A">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" width="25%">Sistema:</td>
            <td><? echo campo_texto('sistema', 'N',  'N', "Sistema", '50', '', '','', '', '', 0, '', '', '', '', ''); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Data Criação:</td>
            <td><? echo campo_texto('dataCriacao', 'N',  'N', "Data Criação", '10', '', '','', '', '', 0, '', '', '', '', ''); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Cadastro realizado por:</td>
            <td><? echo campo_texto('cadastroRealizadoPor', 'N',  'N', "Cadastro realizado por", '50', '', '','', '', '', 0, '', '', '', '', ''); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Total PF:</td>
            <td><? echo campo_texto('totalPf', 'N',  'N', "", '4', 'Total PF', '','', '', '', 0, '', '', '', '', ''); ?></td>
        </tr>
    </table>
</form>
<div id="modalDescricao" style="display: none;">
    <p>
        <span class="campos">
            <textarea id="descricao" readonly="readonly" name="descricao" cols="50" maxlength="300" rows="5" style="width:300px; resize:none;"  class="txareanormal"></textarea>
        </span>
    </p>
</div>

<?php
	$cabecalho     = array('Agrupador','Funcionalidade','Tipo','Qtd ALR/RLR','Qtd TD','Complexidade','QTD PF');
	$tamanho       = array("30%","20%","10%","10%","10%","10%","10%");
	$alinhamento   = array("left","left","center","center","center","center","center");
	$db->monta_lista($sql,$cabecalho,20,5,'N','center',$par2,"",$tamanho,$alinhamento);
?>
<table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" class="alignRight">
			<input id="voltar" type="button" value="Voltar" onclick="btnVoltar()" />
		</td>
	</tr>
</table>
<script type="text/javascript">
    
    $(document).ready(function(){
        $('#modalDescricao').dialog({
            autoOpen    : false,
            dialogClass : 'modalFabrica',
            modal       : true,
            title       : 'Descrição',
            width       : 300,
            resizable   : false,
            draggable   : false,
            buttons     : {
                "Fechar"    : function(){
                    $( this ).dialog("close");
                }
            }
        });
    });

    function abreDescricao( desc, title )
    {
        $('#descricao').val('');
        $('#descricao').val(desc);
        $('#modalDescricao').dialog( "option" , "title" , title )
        $('#modalDescricao').dialog('open');
    }
</script>