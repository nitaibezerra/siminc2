<?php
header( 'Content-Type: text/html; charset=iso-8859-1' );

$painelOperacional  = new PainelOperacional( $db );
$recarregarAction   = !empty($_REQUEST['action'])      ? $_REQUEST['action']          : '' ;
$recarregarSituacao = !empty($_REQUEST['esdid'])       ?  $_REQUEST['esdid']          : '' ;
$recarregarCelid    = !empty($_REQUEST['celid'])       ?  $_REQUEST['celid']          : '' ;
$recarregarSidid    = !empty($_REQUEST['sididForm'])       ?  $_REQUEST['sididForm']         : '' ;

$ordemLista         = !empty($_REQUEST['ordemlista'])  ?  $_REQUEST['ordemlista']     : '' ;

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ . 'www/demandas/_constantes.php';
print '<br/>';

$titulo = "PAINEL OPERACIONAL - Gerente de Projetos";
monta_titulo( $titulo, '&nbsp;' );

$dados = array();

?>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/painel-operacional-gp.js"></script>

<link type="text/css" href="./css/painel_operacional.css" rel="stylesheet" />

<form id="formPainelOperacional" method="post" action="fabrica.php?modulo=principal/painelOperacionalgProjeto&acao=A">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
    <tr>
        <td colspan="2"></td>
    </tr>
    
    <tr >
        <td align='right' class="SubTituloDireita" style="width: 120px">Célula:</td>    
        <td>
            <?php
            $sql = "SELECT
                c.celid AS codigo,
                c.celnome || ' - Gerente: ' || 
                CASE 
                        WHEN u.usunome != '' THEN u.usunome
                        ELSE ' - '
                END as DESCRICAO
               FROM
                demandas.celula c
                LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
                LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
               WHERE
                c.ordid = 1 AND
                c.celstatus = 'A'
               ORDER BY
                c.celnome;";
                //echo $sql;
                $db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --', 'filtraSistema', '','','','','celid','',$recarregarCelid);
            ?>
        </td>
        </tr>
        <tr>
        <td  align='right' class="SubTituloDireita" style="width: 120px">Sistema:</td>
        <td id="listasistema">
            
            <?php 
                    $sql = "
                        SELECT 
					 sidid AS codigo,
					 upper(sidabrev) ||' - '|| siddescricao AS descricao
					FROM 
                                            demandas.sistemadetalhe
					WHERE
					 sidstatus = 'X'
					ORDER BY 
					 siddescricao
                    ";        
                    $db->monta_combo( 'sidid', $sql, 'S', '-- Informe o sistema --', '', '','','','','sidid','',$sididx);
            ?>
            
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width: 80px"></td>
            <td colspan="3">
                <input type="button" id="buttonAtualizar" value="Pesquisar" class="botao">
            </td>
        </tr>
    
</table>
</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td width="50%">
                <br />
        </td>
    </tr>
    <tr>
        <td valign="top" class="center" id="container-painel">
            <?php echo $painelOperacional->painelOperacionalGerenteProjetos($dados); ?>
        </td>
        </td>
    </tr>

    <tr>
        <td colspan="2" id="container-listagem"></td>
    </tr>
</table>






<input type="hidden" name="recarregarAction"   id="recarregarAction"    value="<?php echo($recarregarAction); ?>" />
<input type="hidden" name="recarregarSituacao"    id="recarregarSituacao"  value="<?php echo($recarregarSituacao); ?>" />
<input type="hidden" name="recarregarSidid"       id="recarregarSidid"           value="<?php echo($recarregarSidid); ?>" />
<input type="hidden" name="recarregarCelid"       id="recarregarCelid"           value="<?php echo($recarregarCelid); ?>" />
<input type="hidden" name="ordemlista"            id="ordemlista"          value="<?php echo($ordemLista); ?>" />

<input type="hidden" name="TextrecarregarAction"      id="TextrecarregarAction"    value="<?php echo($recarregarAction); ?>" />
<input type="hidden" name="TextrecarregarSituacao"    id="TextrecarregarSituacao"  value="<?php echo($recarregarSituacao); ?>" />
<input type="hidden" name="TextrecarregarSidid"       id="TextrecarregarSidid"     value="<?php echo($recarregarSidid); ?>" />
<input type="hidden" name="TextrecarregarCelid"       id="TextrecarregarCelid"     value="<?php echo($recarregarCelid); ?>" />
<input type="hidden" name="Textordemlista"            id="Textordemlista"          value="<?php echo($ordemLista); ?>" />


<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script type="text/javascript">
$(document).ready(function(){
        
    PainelOperacionalGerenteProjetoView.init();
    
    <?php if( !empty($recarregarCelid) ){ ?>
    PainelOperacionalGerenteProjetoView.recarregarPagina();
    <?php } ?>
});
</script>