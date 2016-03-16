<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<?php

if($_REQUEST['envioForm']){
    if($_REQUEST['iuscpf'] && $_REQUEST['iusnome'] && $_REQUEST['iusemailprincipal'] && $_REQUEST['pflcod'] && $_REQUEST['tpeqtdbolsa'] && $_SESSION['sisfor']['sifid']){

        $cpf = ereg_replace( "[^0-9]", "", $_REQUEST['iuscpf']);

        // Verifica se existe sisfor.identificacaousuario
        $sql = "select iusd, iusemailprincipal from sisfor.identificacaousuario where iuscpf = '{$cpf}'";
        $ius = $db->pegaLinha($sql);

        $iusd = $ius['iusd'];

        // Se não tiver sisfor.identificacaousuario ele insere. Se tiver, recupera e altera email, caso for.
        if(!$iusd){
            $sql = "insert into sisfor.identificacaousuario (iuscpf, iusnome, iusemailprincipal, iusdatainclusao)
                                                      values('{$cpf}', '{$_REQUEST['iusnome']}', '{$_REQUEST['iusemailprincipal']}', NOW())
                    returning iusd";
            $iusd = $db->pegaUm($sql);
            $db->commit();
        } elseif($ius['iusemailprincipal'] != $_REQUEST['iusemailprincipal']) {
            $sql = "update sisfor.identificacaousuario set
                        iusemailprincipal = '{$_REQUEST['iusemailprincipal']}'
                    where iusd = '{$iusd}'";
            $db->executar($sql);
        }

        // Insere ou altera sisfor.tipoperfil
        if($_REQUEST['tpeid']){
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

monta_titulo('Composição da Equipe','');

$sql = "select p.pfldsc, e.pflcod, e.*
        from sisfor.equipeies e
                inner join seguranca.perfil p on p.pflcod = e.pflcod
        where sifid = '{$_SESSION['sisfor']['sifid']}'";

$dados = $db->carregar($sql);

?>

<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem tabela" style="margin-top: 10px;">
    <thead>
        <tr align="center">
            <td>Perfil</td>
            <td>Valor Total</td>
            <td>Qtd bolsas</td>
            <td>Bolsas Restantes</td>
        </tr>
    </thead>
    <tbody>
        <?php if($dados && is_array($dados)) {
            foreach($dados as $dado){ ?>
            <tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
                <td nowrap="nowrap"><h5><img class="img_detalhe" target="detalhe_<?php echo $dado['pflcod']; ?>" src="/imagens/mais.gif" style="margin-right: 5px;" /><?php echo $dado['pfldsc']; ?></h5></td>
                <td align="right"><?php echo number_format($dado['epivalor'], 2, ',', '.'); ?></td>
                <td align="right"><span id="bolsas_ofertadas_<?php echo $dado['pflcod']; ?>"><?php echo $dado['epiqtd']; ?></span></td>
                <td align="right"><span id="bolsas_restantes_<?php echo $dado['pflcod']; ?>"></span></td>
            </tr>
            <tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';" class="linha_detalhe detalhe_<?php echo $dado['pflcod']; ?>">
                <td colspan="4">
                    <div id="div_equipe_<?php echo $dado['pflcod']; ?>">
                        <?php montarFormularioEquipe($dado['pflcod']); ?>
                    </div>
                </td>
            </tr>
        <?php }
        } else { ?>
            <tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
                <td colspan="4" align="center"><span style="color: red;">Não foi encontrado planejamento de equipe</span></td>
            </tr>
        <?php } ?>
    </tbody>
</table>

<script type="text/javascript">
    jQuery(function(){

        jQuery('.botao_enviar').live('click', function(){
            var valido = true;
            var pflcod = jQuery(this).attr('pflcod');
            jQuery("#formulario_equipe_"+pflcod+" input[type=text]").each(function(i, obj){
                if(!jQuery(obj).val()){
                    valido = false;
                }
            });

            if(!valido){
                alert('Favor preencher todos os campos.');
                return false
            }

            options = {
                success : function() {
                    jQuery("#div_equipe_"+pflcod).load('/sisfor/ajax.php?atualizarFormulario=1&pflcod='+pflcod);
                }
            }
            $form = jQuery("#formulario_equipe_"+pflcod);
            $form.ajaxForm(options);
            $form.submit();
        });

        jQuery('.carregar_dados').live('click', function(){
            var url = jQuery(this).attr('href');
            var pflcod = jQuery(this).attr('pflcod');

            jQuery("#div_equipe_"+pflcod).load(url);
            return false;
        });

        jQuery('.deleta_tipo_perfil').live('click', function(){

            if(confirm('Deseja realmente excluir o registro?')){
                var url = jQuery(this).attr('href');
                var pflcod = jQuery(this).attr('pflcod');

                jQuery("#div_equipe_"+pflcod).load(url);
            }
            return false;
        });

        jQuery('input[tipo=qtd_bolsas]').live('keyup', function(){
            var pflcod = jQuery(this).attr('pflcod');
            var bolsas_restantes = jQuery('#bolsas_restantes_'+pflcod).html();
            var qtd_original = jQuery('#qtd_original_'+pflcod).val() ? jQuery('#qtd_original_'+pflcod).val() : 0;

            if(parseInt(jQuery(this).val()) > (parseInt(bolsas_restantes) + parseInt(qtd_original))){
                if(parseInt(bolsas_restantes)){
                    alert('Existe(m) somente ' + bolsas_restantes + ' bolsa(s) restante(s).');
                } else {
                    alert('Não há mais bolsas restantes.');
                }
                jQuery(this).val('');
            }
        });

        jQuery('input[tipo=cpf_bolsista]').live('change', function(){
            var pflcod = jQuery(this).attr('pflcod');

            if(jQuery(this).val()){
                jQuery.ajax({
                    url: '/sisfor/ajax.php?recuperarUsuario=1&iuscpf='+jQuery(this).val(),
                    dataType: 'json',
                    success: function(dados){
                        console.log(dados);
                        if(dados.nome){
                            jQuery('#iusnome_'+pflcod).val(dados.nome);
                            jQuery('#iusemailprincipal_'+pflcod).val(dados.email);
                        } else {
                            alert('O CPF informado é inválido.');
                            jQuery('#iuscpf_'+pflcod).val('');
                            jQuery('#iusnome_'+pflcod).val('');
                            jQuery('#iusemailprincipal_'+pflcod).val('');
                        }
                    }
                });
            }
        });

        jQuery('.linha_detalhe').hide();
        jQuery('.img_detalhe').click(function(){
            if(jQuery(this).attr('src') == '/imagens/mais.gif'){
                jQuery('.'+jQuery(this).attr('target')).show();
                jQuery(this).attr('src', '/imagens/menos.gif');
                console.log(jQuery('.'+jQuery(this).attr('target')));
            } else {
                jQuery('.'+jQuery(this).attr('target')).hide();
                jQuery(this).attr('src', '/imagens/mais.gif');
                console.log(jQuery('.'+jQuery(this).attr('target')));
            }
        });


    });
</script>