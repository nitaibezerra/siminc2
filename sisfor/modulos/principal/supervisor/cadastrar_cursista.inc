<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<?php
if ($_REQUEST['escolher_outro_curso'] == "true") {

    unset($_SESSION['sisfor']['sifid']);
}


if ($_REQUEST['curso_selecionado']) {

    $_SESSION['sisfor']['sifid'] = $_REQUEST['curso_selecionado'];
}

if (!empty($_SESSION['sisfor']['sifid'])) {
    //echo $_SESSION['sisfor']['sifid'];
//verificarFluxoComposicaoEquipe($_SESSION['sisfor']['docidcomposicaoequipe']);
}



if ($_REQUEST['fpbid']) {

//    $mensarios = recuperarCursista($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);
}


if ($_REQUEST['curso_selecionado']) {

    $_SESSION['sisfor']['sifid'] = $_REQUEST['curso_selecionado'];
}

if($_REQUEST['envioForm']){
    if($_REQUEST['curnome'] && $_REQUEST['curemail'] && $_SESSION['sisfor']['sifid']){

        
        if(!empty($_REQUEST['curcpf'])){
            
        $cpf = ereg_replace( "[^0-9]", "", $_REQUEST['curcpf']);

        // Verifica se existe sisfor.cursista
        $sql = "select curid, curemail from sisfor.cursista where curcpf = '{$cpf}'";
        $cur = $db->pegaLinha($sql);

        $curid = $cur['curid'];
        }
        else{
            $cpf = "";
        }
        // Se não tiver sisfor.cursista ele insere. Se tiver, recupera e altera email, caso for.
        if(!$curid){
            $sql = "insert into sisfor.cursista (curcpf, curnome, curemail)
                                                      values('{$cpf}', '{$_REQUEST['curnome']}', '{$_REQUEST['curemail']}')
                    returning curid";
            $curid = $db->pegaUm($sql);
            $db->commit();
        } elseif($cur['curemail'] != $_REQUEST['curemail']) {
            $sql = "update sisfor.cursista set
                        curemail = '{$_REQUEST['curemail']}'
                    where curid = '{$curid}'";
            $db->executar($sql);
        }

        // Insere ou altera sisfor.cursistacurso
        if($_REQUEST['cucid']){
            $sql = "update sisfor.cursistacurso set
                        cucstatus = '{$_REQUEST['cucstatus']}',
                    where cucid = '{$_REQUEST['cucid']}'";

            $db->executar($sql);

        } else {
        	
        	$cucid = $db->pegaUm("SELECT cucid from sisfor.cursistacurso where curid='{$curid}' and sifid='{$_SESSION['sisfor']['sifid']}' and fpbid='{$_REQUEST['fpbid']}'");
        	
        	if(!$cucid) {
        	
	            $sql = "insert into sisfor.cursistacurso (curid, sifid, fpbid, estuf, muncod, iusdavaliador)
	                                            values('{$curid}', '{$_SESSION['sisfor']['sifid']}', '{$_REQUEST['fpbid']}', '{$_REQUEST['estuf']}', '{$_REQUEST['muncod_nascimento']}', '{$_REQUEST['iusdavaliador']}') returning cucid";
	            $cucid = $db->pegaUm($sql);
            
        	} else {
        		
        		$sql = "update sisfor.cursistacurso set estuf=".(($_REQUEST['estuf'])?"'".$_REQUEST['estuf']."'":"NULL").", muncod=".(($_REQUEST['muncod_nascimento'])?"'".$_REQUEST['muncod_nascimento']."'":"NULL").", iusdavaliador='{$_REQUEST['iusdavaliador']}' where cucid='{$cucid}'";
        		
        		$db->executar($sql);
        		
        	}
        }

        $db->commit();
    }
    die;
}

if ('gravar' == $_REQUEST['acao']) {

    if (isset($_REQUEST['avaliacao']) && is_array($_REQUEST['avaliacao'])) {
        foreach ($_REQUEST['avaliacao'] as $avaliacao) {
            $cucid = $avaliacao['cucid'] ? $avaliacao['cucid'] : null;
            $cavid = $avaliacao['cavid'] ? $avaliacao['cavid'] : null;
            
            if(!$cavid && $cucid) $cavid = $db->pegaUm("SELECT cavid FROM sisfor.cursistaavaliacoes WHERE cucid='".$cucid."'");
            
            $cavsituacao = $avaliacao['cavsituacao'] ? $avaliacao['cavsituacao'] : null;
            $cavparticipacao = $avaliacao['cavparticipacao'] ? str_replace(array('.', ','), array('', '.'), $avaliacao['cavparticipacao']) : 0;
            $cavatividadesrealizadas = $avaliacao['cavatividadesrealizadas'] ? str_replace(array('.', ','), array('', '.'), $avaliacao['cavatividadesrealizadas']) : 0;
//            $iusdorientador = $avaliacao['iusdorientador'] ? $avaliacao['iusdorientador'] : null;
//            $cavtotal = $avaliacao['cavtotal'] ? str_replace(array('.', ','), array('', '.'), $avaliacao['cavtotal']) : null;

            if ($cavid) {
                $sql = " update sisfor.cursistaavaliacoes set
                                cavparticipacao = $cavparticipacao,
                                cavatividadesrealizadas = $cavatividadesrealizadas,
                                cavsituacao = '$cavsituacao'
                         where cavid = $cavid ";
            } else {
                $sql = " INSERT INTO sisfor.cursistaavaliacoes(cucid, cavparticipacao,cavatividadesrealizadas, cavsituacao)
                                VALUES ($cucid, $cavparticipacao, $cavatividadesrealizadas, '$cavsituacao') ";
            }

            $db->executar($sql);
            $db->commit();
        }
    }

    $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid={$_REQUEST['fpbid']}";
    $al = array("alert" => "Operação realizada com sucesso!", "location" => $location);
    alertlocation($al);
}

monta_titulo('Cadastro de Cursistas', '');

if (empty($_SESSION['sisfor']['sifid'])) {


    $sql = "SELECT '<input type=radio >' as radio, p.pfldsc, uniabrev||' - '||unidsc as universidade,
                                                                          CASE WHEN s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
                                                                               WHEN s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
                                                                               WHEN s.ocuid is not null then oc.ocunome end as curso, s.sifid as id_curso
             
                                                            FROM sisfor.tipoperfil t 
                                                            INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
                                                            INNER JOIN sisfor.sisfor s ON s.sifid = t.sifid 
                                                            INNER JOIN public.unidade u on u.unicod = s.unicod
                                                            LEFT JOIN catalogocurso2014.iesofertante ieo ON ieo.ieoid = s.ieoid 
                                                            LEFT JOIN catalogocurso2014.curso cur on cur.curid = ieo.curid 
                                                            LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
                                                            LEFT JOIN catalogocurso2014.curso cur2 on cur2.curid = cnv.curid 
                                                            LEFT JOIN sisfor.outrocurso oc on oc.ocuid = s.ocuid 
                                                            WHERE t.iusd='" . $_SESSION['sisfor']['iusd'] . "'";
    $lista_cursos = $db->carregar($sql);
    if (!empty($lista_cursos)) {
        ?>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td colspan="2" class="SubTituloCentro">
                    <form action="" method="post" id="form_curso_selecionado">
                        <select name="curso_selecionado" id="curso_selecionado">
                            <option value="">Selecione um curso</option>
        <?php foreach ($lista_cursos as $key) { ?>
                                <option value="<?php echo $key[id_curso] ?>"><?php echo $key[curso] ?></option>
                            <?php } ?>
                        </select>

                        <input type="button" name="enviar" value="Enviar" onclick="if(document.getElementById('curso_selecionado').value==''){alert('Selecione um curso');return false;}document.getElementById('form_curso_selecionado').submit();">
                    </form>
            </tr>
        </table>
        



        <?php
    }
}
?>


<?php if (!empty($_SESSION['sisfor']['sifid'])) { ?>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" style="text-align:right">
                <form action="sisfor.php?modulo=principal/supervisor/supervisor&acao=A&aba=cadastrar_cursista" method="post">
                    <input type="hidden" name="escolher_outro_curso" value="true">
                    <input type="submit" value="Escolher outro curso">


                </form>

            </td>
        </tr>
    </table>

<?php } ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2" class="SubTituloCentro">
<?php
if (!empty($_SESSION['sisfor']['sifid'])) {

    carregarPeriodoReferenciaCursista(array('sifid' => $_SESSION['sisfor']['sifid'], 'fpbid' => $_REQUEST['fpbid']));
}
?>
        </td>
    </tr>
</table>

<div id="div_cursista">
<?php
if ($_REQUEST['fpbid']) {
    montarFormularioCursista($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid'], $_SESSION['sisfor']['iusd']);
}
?>
</div>

<script type="text/javascript">
    jQuery(function() {

        jQuery('.botao_enviar').live('click', function(){


            var valido = true;
            jQuery("#formulario_cursista input[type=text]").each(function(i, obj){
                if(!jQuery(obj).val() && jQuery(obj).attr('name') != "curcpf"){
                    valido = false;
                }
            });

            if(!valido){
                alert('Favor preencher todos os campos.');
                return false
            }

            options = {
                success : function() {
                    jQuery("#div_cursista").load('/sisfor/ajax.php?atualizarFormularioCursista=1&sifid='+'<?php echo $_SESSION['sisfor']['sifid'] . '&fpbid=' .$_REQUEST['fpbid']?>');
                }
            }
            $form = jQuery("#formulario_cursista");
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

        jQuery('#curcpf').live('blur', function() {

            if (jQuery(this).val()) {
                jQuery.ajax({
                    url: '/sisfor/ajax.php?recuperarUsuario=1&iuscpf=' + jQuery(this).val(),
                    dataType: 'json',
                    success: function(dados) {
                        if (dados.nome) {
                            jQuery('#curnome').val(dados.nome);
                            jQuery('#curemail').val(dados.email);
                        } else {
                            alert('O CPF informado é inválido.');
                            jQuery('#curnome, #curcpf, #curemail').val('');
                        }
                    }
                });
            }
        });
    });
</script>