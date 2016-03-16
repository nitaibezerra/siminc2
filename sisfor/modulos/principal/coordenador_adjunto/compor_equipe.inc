<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<?php
verificarFluxoValidado($_SESSION['sisfor']['docidprojeto']);

if ($_REQUEST['envioForm']) {
    if ($_REQUEST['iuscpf'] && $_REQUEST['iusnome'] && $_REQUEST['iusemailprincipal'] && $_REQUEST['pflcod'] && $_REQUEST['tpeqtdbolsa'] && $_SESSION['sisfor']['sifid']) {

        $cpf = ereg_replace("[^0-9]", "", $_REQUEST['iuscpf']);

        // Verifica se existe sisfor.identificacaousuario
        $sql = "select iusd, iusemailprincipal from sisfor.identificacaousuario where iuscpf = '{$cpf}'";
        $ius = $db->pegaLinha($sql);

        $iusd = $ius['iusd'];

        // Se não tiver sisfor.identificacaousuario ele insere. Se tiver, recupera e altera email, caso for.
        if (!$iusd) {
            $sql = "insert into sisfor.identificacaousuario (iuscpf, iusnome, iusemailprincipal, iusdatainclusao)
                                                      values('{$cpf}', '{$_REQUEST['iusnome']}', '{$_REQUEST['iusemailprincipal']}', NOW())
                    returning iusd";
            $iusd = $db->pegaUm($sql);
            $db->commit();
        } elseif ($ius['iusemailprincipal'] != $_REQUEST['iusemailprincipal']) {
            $sql = "update sisfor.identificacaousuario set
                        iusemailprincipal = '{$_REQUEST['iusemailprincipal']}'
                    where iusd = '{$iusd}'";
            $db->executar($sql);
        }

        // Insere ou altera sisfor.tipoperfil
        if ($_REQUEST['tpeid']) {
            $sql = "update sisfor.tipoperfil set
                        iusd = '{$iusd}',
                        tpeqtdbolsa = '{$_REQUEST['tpeqtdbolsa']}'
                    where tpeid = '{$_REQUEST['tpeid']}'";

            $db->executar($sql);
        } else {
            $sql = "insert into sisfor.tipoperfil (iusd, pflcod, tpebolsa, sifid, tpeqtdbolsa)
                                            values('{$iusd}', '{$_REQUEST['pflcod']}', 't', '{$_SESSION['sisfor']['sifid']}', '{$_REQUEST['tpeqtdbolsa']}') returning tpeid";
            $tpeid = $db->pegaUm($sql);
        }

        $db->commit();
    }
    die;
}

monta_titulo('Composição da Equipe', '');



$sql1 = "select tp.tpeid, iu.iusnome, iu.iuscpf, iu.iusemailprincipal, tp.tpeqtdbolsa
            from sisfor.tipoperfil tp
            inner join sisfor.identificacaousuario iu on iu.iusd = tp.iusd
            where tp.sifid = '{$_SESSION['sisfor']['sifid']}'
            and tp.pflcod = '1105'";
$dados1 = $db->carregar($sql1);



if (!$dados1) {
    

    $sql2 = "select p.pfldsc, e.pflcod, round(pp.plpvalor,2)::text valorunitario, e.*
        from sisfor.equipeies e
        inner join seguranca.perfil p on p.pflcod = e.pflcod
        inner join sisfor.pagamentoperfil pp on pp.pflcod = e.pflcod
        where sifid = '{$_SESSION['sisfor']['sifid']}' and pp.pflcod = '1105'
        order by e.epiid ";
    $dados2 = $db->pegaLinha($sql2);
    $qt_bolsas_coordenador_curso = $dados2['epiqtd'];

    $cpf = $db->pegaUm("SELECT replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.') FROM sisfor.sisfor s INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
    
   
            


    $cpf = ereg_replace("[^0-9]", "", $cpf);

    // Verifica se existe sisfor.identificacaousuario
    $sql3 = "select iusd, iusnome, iusemailprincipal from sisfor.identificacaousuario where iuscpf = '{$cpf}'";
    $ius = $db->pegaLinha($sql3);

    $iusd = $ius['iusd'];
    if (!$iusd) {
        $sql5 = "insert into sisfor.identificacaousuario (iuscpf, iusnome, iusemailprincipal, iusdatainclusao) values('{$cpf}', '{$ius['iusnome']}', '{$ius['iusemailprincipal']}', '{$ius['iusemailprincipal']}', NOW())
                    returning iusd";
        $iusd = $db->pegaUm($sql5);
        $db->commit();
    }

    $sql4 = "insert into sisfor.tipoperfil (iusd, pflcod, tpebolsa, sifid, tpeqtdbolsa)
                                          values('{$iusd}', '1105', 't', '{$_SESSION['sisfor']['sifid']}', '$qt_bolsas_coordenador_curso') returning tpeid";
    $tpeid = $db->pegaUm($sql4);
    $db->commit();
}






$sql = "select p.pfldsc, e.pflcod, round(pp.plpvalor,2)::text valorunitario, e.*
        from sisfor.equipeies e
                inner join seguranca.perfil p on p.pflcod = e.pflcod
                inner join sisfor.pagamentoperfil pp on pp.pflcod = e.pflcod
        where sifid = '{$_SESSION['sisfor']['sifid']}'
        order by e.epiid ";

$dados = $db->carregar($sql);
?>

<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
    <tr>
        <td>

            <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem tabela" style="margin-top: 10px;">
                <thead>
                    <tr align="center">
                        <td>Perfil</td>
                        <td>Valor Unitário</td>
                        <td>Valor Total</td>
                        <td>Qtd bolsas</td>
                        <td>Bolsas não vinculadas</td>
                    </tr>
                </thead>
                <tbody>
<?php if ($dados && is_array($dados)) {
    foreach ($dados as $dado) {
        ?>
                            <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                                <td nowrap="nowrap">
                                    <h5>
        <?php if ($dado['epiqtd']) { ?>
                                            <img class="img_detalhe" target="detalhe_<?php echo $dado['pflcod']; ?>" src="/imagens/mais.gif" style="margin-right: 5px;" />
        <?php } ?>
        <?php echo $dado['pfldsc']; ?>
                                    </h5>
                                </td>
                                <td align="right"><?php echo number_format($dado['valorunitario'], 2, ',', '.'); ?></td>
                                <td align="right"><?php echo number_format($dado['epivalor'], 2, ',', '.'); ?></td>
                                <td align="right"><span id="bolsas_ofertadas_<?php echo $dado['pflcod']; ?>"><?php echo $dado['epiqtd']; ?></span></td>
                                <td align="right"><span id="bolsas_restantes_<?php echo $dado['pflcod']; ?>"></span></td>
                            </tr>
                            <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';" class="linha_detalhe detalhe_<?php echo $dado['pflcod']; ?>">
                                <td colspan="5">
                                    <div id="div_equipe_<?php echo $dado['pflcod']; ?>">
                                        <?php montarFormularioEquipe($dado['pflcod']); ?>
                                    </div>
                                </td>
                            </tr>
    <?php }
} else {
    ?>
                        <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                            <td colspan="4" align="center"><span style="color: red;">Não foi encontrado planejamento de equipe</span></td>
                        </tr>
                                <?php } ?>
                </tbody>
            </table>

        </td>
        <td width="5%" valign="top">
                    <?
                    /* Barra de estado atual e ações e Historico */
                    wf_desenhaBarraNavegacao($_SESSION['sisfor']['docidcomposicaoequipe'], array("sifid" => $_SESSION['sisfor']['sifid']));
                    ?>
        </td>
    </tr>
</table>

<script type="text/javascript">
    jQuery(function() {

        jQuery('.botao_enviar').live('click', function() {
            var valido = true;
            var pflcod = jQuery(this).attr('pflcod');
            jQuery("#formulario_equipe_" + pflcod + " input[type=text]").each(function(i, obj) {
                if (!jQuery(obj).val()) {
                    valido = false;
                }
            });

            if (!valido) {
                alert('Favor preencher todos os campos.');
                return false
            }

            options = {
                success: function() {
                    jQuery("#div_equipe_" + pflcod).load('/sisfor/ajax.php?atualizarFormulario=1&pflcod=' + pflcod);
                }
            }
            $form = jQuery("#formulario_equipe_" + pflcod);
            $form.ajaxForm(options);
            $form.submit();
        });

        jQuery('.carregar_dados').live('click', function() {
            var url = jQuery(this).attr('href');
            var pflcod = jQuery(this).attr('pflcod');

            jQuery("#div_equipe_" + pflcod).load(url);
            return false;
        });

        jQuery('.deleta_tipo_perfil').live('click', function() {

            if (confirm('Deseja realmente excluir o registro?')) {
                var url = jQuery(this).attr('href');
                var pflcod = jQuery(this).attr('pflcod');

                jQuery("#div_equipe_" + pflcod).load(url);
            }
            return false;
        });

        jQuery('input[tipo=qtd_bolsas]').live('keyup', function() {
            var pflcod = jQuery(this).attr('pflcod');
            var bolsas_restantes = jQuery('#bolsas_restantes_' + pflcod).html();
            var qtd_original = jQuery('#qtd_original_' + pflcod).val() ? jQuery('#qtd_original_' + pflcod).val() : 0;

            if (parseInt(jQuery(this).val()) > (parseInt(bolsas_restantes) + parseInt(qtd_original))) {
                if (parseInt(bolsas_restantes)) {
                    alert('Existe(m) somente ' + bolsas_restantes + ' bolsa(s) restante(s).');
                } else {
                    alert('Não há mais bolsas restantes.');
                }
                jQuery(this).val('');
            }
        });

        jQuery('input[tipo=cpf_bolsista]').live('blur', function() {

            var pflcod = jQuery(this).attr('pflcod');
            var cpf = jQuery(this).val();

            if (cpf) {
                jQuery.ajax({
                    url: '/sisfor/ajax.php?verificaUsuarioDuplicado=1&iuscpf=' + cpf,
                    dataType: 'json',
                    success: function(dados) {
                        if (dados.permitido) {
                            jQuery.ajax({
                                url: '/sisfor/ajax.php?recuperarUsuario=1&iuscpf=' + cpf,
                                dataType: 'json',
                                success: function(dados) {
                                    console.log(dados);
                                    if (dados.nome) {
                                        jQuery('#iusnome_' + pflcod).val(dados.nome);
                                        jQuery('#iusemailprincipal_' + pflcod).val(dados.email);
                                    } else {
                                        alert('O CPF informado é inválido.');
                                        jQuery('#iuscpf_' + pflcod).val('');
                                        jQuery('#iusnome_' + pflcod).val('');
                                        jQuery('#iusemailprincipal_' + pflcod).val('');
                                    }
                                }
                            });
                        } else {
                            alert(dados.mensagem);
                            jQuery('#iuscpf_' + pflcod).val('');
                        }
                    }
                });
            }
        });

        jQuery('.linha_detalhe').hide();
        jQuery('.img_detalhe').click(function() {
            if (jQuery(this).attr('src') == '/imagens/mais.gif') {
                jQuery('.' + jQuery(this).attr('target')).show();
                jQuery(this).attr('src', '/imagens/menos.gif');
                console.log(jQuery('.' + jQuery(this).attr('target')));
            } else {
                jQuery('.' + jQuery(this).attr('target')).hide();
                jQuery(this).attr('src', '/imagens/mais.gif');
                console.log(jQuery('.' + jQuery(this).attr('target')));
            }
        });


    });
</script>