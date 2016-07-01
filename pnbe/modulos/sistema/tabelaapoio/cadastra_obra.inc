<?
if($_REQUEST['function']) {
    $_REQUEST['function']($_REQUEST); 
}

if( $_POST['obrid'] != ''){
    $sql = "SELECT *
            FROM pnbe.obra
            WHERE obrstatus = 'A'
            AND obrid =".$_POST['obrid'];
    $dados = $db->pegaLinha( $sql );
    extract($dados);
}

// if($_GET['obrid'] != ''){
//    $_SESSION['livro']['obrid'] = $_GET['obrid'];
//    $obrid = $_GET['obrid'];
//    $sql = "SELECT *
//            FROM pnbe.obra
//            WHERE obrstatus = 'A'
//            AND obrid =".$obrid;
//
//    $param = $db->pegaLinha( $sql );
//    extract($param);
//}


//Chamada de programa
include_once  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('PNBE - Programa Nacional da Biblioteca na Escola', 'CADASTRO DE OBRA');
?>
<style type="text/css">
    .marcado{background-color:#C1FFC1!important}
    .remover{display:none}
    .filtros{clear:both}
    .control-label.filtro{margin-bottom:3px}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
    jQuery(document).ready(function(){
        //criando filtro dinâmico de pesquisa
        $.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };
        //recuperando string digitada no campo Pesquisar
        $("#textFind").keyup(function()
        {
            $('table.listagem tbody tr td').removeClass('marcado');
            $('table.listagem tbody tr').removeClass('remover');
            stringPesquisa = $("#textFind").val();
            if (stringPesquisa) {
                $('table.listagem tbody tr td:contains('+stringPesquisa+')').addClass('marcado');
                $('table.listagem tbody tr').not(':contains('+stringPesquisa+')').addClass('remover');
            }
        });
        
        $('#salvarObras').click(function(){
            //validando campos
            if( $('#obrcodigo').val() == '')
            {
                alert('O Campo "Código da obra" é Obrigatório');
                $( "#obrcodigo" ).focus();
                return false;
            }
            if( $('#obrtitulo').val() == '')
            {
                alert('O Campo "Título da obra" é Obrigatório');
                $( "#obrtitulo" ).focus();
                return false;
            }
            if( $('#catid').val() == '')
            {
                alert('O Campo "Categoria" é Obrigatório');
                $( "#catid" ).focus();
                return false;
            }
            if( $('#ediid').val() == '')
            {
                alert('O Campo "Editora" é Obrigatório');
                $( "#ediid" ).focus();
                return false;
            }
            if( $('#obrano').val() == '')
            {
                alert('O Campo "Editora" é Obrigatório');
                $( "#obrano" ).focus();
                return false;
            }
            
            $('#function').val('salvarObras');
            $('#formulario_param').submit();
        });
    });

    function voltarInicio(){
        window.location.href = 'pnbe.php?modulo=inicio&acao=C';
    }
    
    function alterarObras(obrid,del){
        $('#obrid').val(obrid);
        $('#del').val(del);
        if(del){
            $('#function').val('salvarObras');
        }
        $('#formulario_param').submit();
    }

</script>
<div id="loader-container" style="position: absolute; background-color: white; 
	 opacity: .6; width:110%; height:2000%; margin-top: -200px; 
	 margin-left: -20px; Z-index:22; display:none;" >
</div>

<form id="formulario_param" name="formulario_param" function="" method="post" >
    <input type="hidden" name="function" id="function" value=""/>
    <input type="hidden" name="obrid" id="obrid" value="<?=$_REQUEST['obrid']?>"/>
    <input type="hidden" name="del" id="del" value=""/>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita">Código da Obra</td>
                <td>
                    <? echo campo_texto('obrcodigo', 'S', 'S', 'Código da Obra', 20, 20, "", "", '', '', 0, 'id="obrcodigo"', '', $obrcodigo, '', null );?> 
                </td>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Título da Obra:</td>
            <td>
                <? echo campo_texto('obrtitulo', 'S', 'S', 'Título da Obra', 50, 200, "", "", '', '', 0, 'id="obrtitulo"', '', $obrtitulo, '', null ); ?>
            </td>
	</tr>
        <tr>
            <td class="SubTituloDireita"> Categoria:</td>
            <td>
                <?php
                $sql = "SELECT catid as codigo, catdescricao as descricao FROM pnbe.categoria";

                $db->monta_combo("catid", $sql, 'S', 'Selecione...', '', '','',180, 'S','catid','', $catid, 'Categoria');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Editora:</td>
            <td>
                <?php
                $sql = "SELECT ediid as codigo, edirazaosocial as descricao FROM pnbe.editora";

                $db->monta_combo("ediid", $sql, 'S', 'Selecione...', '', '','', 180, 'S','ediid','', $ediid, 'Editora');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Autor(es):</td>
            <td>
                <? echo campo_texto('obrautor', 'S','S', 'Autor', 200, 1900, '', '', '', '', 0, 'id="obrautor"', '', $obrautor, '', null ); ?>
            </td>
	</tr>
        <tr>
            <td class="SubTituloDireita">Ano:</td>
            <td>
                <?= campo_texto( 'obrano', 'S', 'S', 'Ano', 5, 4, '####', '', '', '', 0, 'id="obrautor"', '', $obrano, '', null  ); ?>
            </td>
	</tr>
        <tr>
            <td colspan="2">
                <center>
                    <!--<input type="button" id="btnSalvar" name="btnSalvar" value="Salvar" onclick="salvaLivro('salvar','');"/>-->
                    <input type="button" id="salvarObras" name="salvarObras" value="Salvar"/>
                    <input type="button" id="voltar" name="voltar" value="Voltar" onclick="voltarInicio();"/>
                </center>
            </td>
        </tr
        <tr>
            <td colspan="2">
                <br style="clear:both" />
                    <label for=textFind title="Pesquisa apenas os dados que já estão sendo exibidos na tabela!">Pesquisa rápida: </label>
                    <input class="normal form-control" type="text" id="textFind" title="Pesquisa apenas os dados que já estão sendo exibidos na tabela!">
                <br style="clear:both" />
            </td>
        </tr>
    </table>
</form>
<?
$sql = "SELECT 
                    '<center>
                        <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" id=\"' || obrid || '\" class=\"alterar\">
                            <img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" onclick=\"alterarObras(' || obrid || ',0)\">
                        </a>
                        <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" id=\"' || obrid || '\" class=\"excluir\">
                            <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" onclick=\"alterarObras(' || obrid || ',1)\">
                        </a>
                    </center>' as acoes, 
			obrcodigo, 
			obrtitulo, 
			catdescricao,
			edirazaosocial,  
			obrautor,
                        obrano
		FROM 
			pnbe.obra o
		INNER JOIN 
                        pnbe.categoria c ON c.catid = o.catid
                INNER JOIN 
                        pnbe.editora e ON e.ediid = o.ediid
                WHERE obrstatus = 'A'";
//ver(simec_htmlentities($sql));
$cabecalho = array( "Ações","Código da Obra","Título da Obra" , "Categoria", "Editora", "Autor", "Ano");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>
