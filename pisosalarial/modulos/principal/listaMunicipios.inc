<?php
if($_POST['gravaMuncodSessao'] == 'true'){

    $arTerritorio = explode('_', $_POST['muncod_estuf']);

    $_SESSION['piso']['muncod'] = $arTerritorio[0];
    $_SESSION['piso']['estuf']  = $arTerritorio[1];
    exit;
}

include_once APPPISO . 'classes/Municipio.class.inc';
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Lista Municípios', 'Filtros' );
$obMunicipio = new Municipio();
unset($_SESSION['piso']['muncod']);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
<!--
jQuery.noConflict();

jQuery(document).ready(function(){
    jQuery('.visualiza_piso').click(function(){
        jQuery.ajax({
            url: document.location.href,
            type: 'post',
            data: 'gravaMuncodSessao=true&muncod_estuf='+this.id,
            success: function(e){
//                alert(e);
                document.location.href = 'pisosalarial.php?modulo=principal/gestaoderecursos/adicionar&acao=A';
            }
        });
    });

    jQuery('.limpar_formulario').click(function(){
        jQuery('#formulario').each(function(){
            for(x=0;x<this.length;x++){
                if(this[x].type != 'button' &&
                   this[x].type != 'submit'){

                    this[x].value = '';
                }
                if(this[x].type == 'checkbox'){
                    this[x].checked = false;
                }
            }
        });

    });
});
//-->
</script>
<form action="" method="post" id="formulario">
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td valign="bottom">
                Município
                <br/>
                <?php
                    $filtro = simec_htmlentities( $_POST['municipio'] );
                    $municipio = $filtro;
                    echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td valign="bottom">
                Estado
                <br/>
                <?php
                $estuf = $_POST['estuf'];
                $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e where e.estuf in ('AL', 'BA', 'CE', 'MA', 'PB', 'PE', 'PI', 'RN', 'AM', 'PA') order by e.estdescricao asc";
                $db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td valign="bottom">
                <input type="checkbox" name="gestaorecursos" <?php echo isset($_POST['gestaorecursos']) ? 'checked' : ''; ?>/>&nbsp;
                Possui pelo menos um arquivo em gestão de recusros
                <br/>
                <input type="checkbox" name="planocarreira" <?php echo isset($_POST['planocarreira']) ? 'checked' : ''; ?>/>&nbsp;
                Possui pelo menos um arquivo em plano de carreira
                <br/>
                <input type="checkbox" name="folhapagamento" <?php echo isset($_POST['folhapagamento']) ? 'checked' : ''; ?>/>&nbsp;
                Possui pelo menos uma folha de pagamento
            </td>
        </tr>
        <tr>
            <td>
                <input type="submit" value="Pesquisar">
                <input type="button" value="Limpar" class="limpar_formulario">
            </td>
        </tr>
    </table>
</form>
<?php $obMunicipio->listaMunicipios($_POST) ?>