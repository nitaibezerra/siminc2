<?php
header( 'content-type: text/html; charset=iso-8859-1;' );
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Titulo da página abaixo da Aba (modificado)
monta_titulo( "INVENTÁRIO", "<b>Alterar Inventário</b>" );

$coInventario   = $_REQUEST['co_inventario'];

if(empty($coInventario)){
?>   
    <script type="text/javascript">
        alert('Erro a acessar a página, por favor acesse novamente');
        history.go(-1);
    </script>
<?php    
    exit;
}

$perfis = arrayPerfil();

$sql = "SELECT '<a href=\"fabrica.php?modulo=sistema/geral/inventario/editarFuncionalidade&acao=A&co_inventario='|| func.co_inventario ||'&co_funcionalidade='|| func.co_funcionalidade ||' \">
                    <img style=\"border: none;cursor:pointer;\" src=\"/imagens/editar_nome.gif\" alt=\"Editar\" title=\"Editar\" />
                </a>
                
                <a onclick=\"removerFuncionalidade('|| func.co_funcionalidade ||')\">
                    <img style=\"cursor:pointer; border: none;\" src=\"/imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" />
                </a>',
                '<span>'|| agrup.no_agrupador ||'</span>' AS Agrupador, 
               '<span>'||ds_funcionalidade ||'</span>' AS Funcionalidade, 
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

     
$sqlInventario  = "SELECT inv.co_inventario, to_char(inv.dt_cadastro,'DD/MM/YYYY') as dt_cadastro, sid.sidid,
                    sid.siddescricao, usu.usunome, usu.usucpf, inv.dt_encerramento, inv.usucpfencerramento
                    , SUM( func.qtd_pf ) as total_pf
          FROM inventario.tb_simec_inventario inv
          INNER JOIN demandas.sistemadetalhe sid
            ON inv.sidid = sid.sidid   
          INNER JOIN seguranca.usuario usu
            ON  inv.usucpf = usu.usucpf
          LEFT JOIN inventario.tb_simec_funcionalidade func
            ON inv.co_inventario = func.co_inventario
     WHERE inv.co_inventario = {$coInventario}
     GROUP BY inv.co_inventario, inv.dt_cadastro, sid.sidid,
                    sid.siddescricao, usu.usunome, usu.usucpf, inv.dt_encerramento, inv.usucpfencerramento";
     

$dadosInventario = $db->pegaLinha($sqlInventario);

$sistema                = $dadosInventario['siddescricao'];
$dataCriacao            = $dadosInventario['dt_cadastro'];
$cadastroRealizadoPor   = $dadosInventario['usunome'];
$totalPf                = $dadosInventario['total_pf'];
$estaEncerrado          = (!empty( $dadosInventario['dt_encerramento']) && !empty($dadosInventario['usucpfencerramento'] ) ) ? true : false;

if($estaEncerrado){
?>   
    <script type="text/javascript">
        alert('Não é possível alterar um inventário já encerrado.');
        history.go(-1);
    </script>
<?php    
    exit;
}

?>

<link rel='stylesheet' type='text/css' href='./css/jquery-ui-1.8.16.custom.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>



<form id="form-listaAuditoriaGc" method="post" action="?modulo=principal/gerencia-configuracao/listar&acao=A" >
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
            <textarea id="descricao" readonly="readonly" name="descricao" cols="50" maxlength="300" rows="5" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); " style="width:50ex; resize:none;"  class="txareanormal"></textarea>
        </span>
    </p>
</div>

<?php
    $cabecalho     = array('Ações','Agrupador','Funcionalidade','Tipo','Qtd ALR/RLR','Qtd TD','Complexidade','QTD PF');
    $tamanho       = array("10%","20%","20%","10%","10%","10%","10%","10%");
    $alinhamento   = array("center","left","left","center","center","center","center", 'center');
    $db->monta_lista($sql,$cabecalho,20,5,'N','center',$par2,"",$tamanho,$alinhamento);
?>

<?php if( in_array(PERFIL_FISCAL_CONTRATO, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( PERFIL_CONTAGEM_PF, $perfis ) ) { ?>
<table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2" class="alignLeft">
            <input type="button" value="Inserir Nova Funcionalidade" onclick='parent.location="fabrica.php?modulo=sistema/geral/inventario/inserirFuncionalidade&acao=A&co_inventario=<?php echo $coInventario ?>"' />
            <input id="encerrarInventario" name="encerrarInventario" type="button" value="Encerrar Inventário"  />
            <input id="co_inventario" name="co_inventario" type="hidden" value="<?php echo $coInventario ?> " />
        </td>
    </tr>
</table>
<?php }?>

<script type="text/javascript">

    $(document).ready(function(){
        $('#modalDescricao').dialog({
            autoOpen    : false,
            dialogClass : 'modalFabrica',
            modal       : true,
            title       : 'Descrição',
            width       : 300,
            resizable   : false,
            draggable   : true,
            buttons     : {
                "Fechar"    : function(){
                    $( this ).dialog("close");
                }
            }
        });
        
        $('#encerrarInventario').click(function(){
            var coInventario = $('#co_inventario').val();
            encerrarInventario( coInventario );
        });
        
    });
    
    function abreDescricao( desc, title )
    {
        $('#descricao').val('');
        $('#descricao').val(desc);
        $('#modalDescricao').dialog( "option" , "title" , title )
        $('#modalDescricao').dialog('open');
    }

   
    function removerFuncionalidade( coFuncionalidade )
    {
        var param = { 'co_funcionalidade' : coFuncionalidade };
        if(coFuncionalidade == '')
        {
            return false;
        }
        
        if(confirm( 'Confirma a exclusão da funcionalidade?' ))
        {
            $.post( 'geral/inventario/remover_funcionalidade.php', param, function(response){
                
                alert( response.msg );
                
                if( response.status == true )
                {
                    window.location.reload();
                }
                
            }, 'json' );
            
        }

        return true;
    }
    
    function encerrarInventario( coInventario )
    {
        var param = { 'co_inventario' : coInventario };
        if(coInventario == '')
        {
            return false;
        }
        
        if(confirm( "Após o encerramento, o inventário não poderá mais ser alterado.\nConfirma a operação?" ))
        {
            $.post( 'geral/inventario/encerrar_inventario.php', param, function(response){
                
                alert( response.msg );
                
                if( response.status == true )
                {
                    window.location = 'fabrica.php?modulo=sistema/geral/inventario/visualizarInventario&acao=A&co_inventario='+ coInventario;
                }
                
            }, 'json' );
        }
        return true;
    }
</script>