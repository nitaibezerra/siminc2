<?php
if (!$_SESSION["exercicio"]) {
    echo '<script>
			/*** Exibe o alerta de erro ***/
			alert("Ocorreu um erro interno.\n
			       O sistema irá redirecioná-lo à página inicial do módulo.");
			       
			/*** Redireciona o usuário ***/
			location.href = "pdeescola.php?modulo=inicio&acao=C";
		  </script>';
    die();
}


if ($_REQUEST['pesquisa']) {
    
    unset($_SESSION['maiseducacao']['filtro']);

    if ($_POST['tpcid']) {
        if ($_POST['tpcid'] == "1" && $_POST['estuf']) {
            $_SESSION['maiseducacao']['filtro']['tpcid'] = $_POST['tpcid'];
            $_SESSION['maiseducacao']['filtro']['estuf'] = $_POST['estuf'];
        }
        if ($_POST['tpcid'] == "3" && $_POST['estuf']) {
            $_SESSION['maiseducacao']['filtro']['tpcid'] = $_POST['tpcid'];
            $_SESSION['maiseducacao']['filtro']['estuf'] = $_POST['estuf'];
            $_SESSION['maiseducacao']['filtro']['muncod'] = $_POST['muncod'];
        }
    }
}

unset($_SESSION['minc']['mceid']);
unset($_SESSION['minc']['entid']);
?>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script src="./js/meajax.js" type="text/javascript"></script>
<script>
    //redirecionaME('meajax.php', 'tipo=redirecioname&entid='+<?php echo $entidade; ?>);
</script>							  	
<?php
if ($_POST['ajaxestuf']) {
    $sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '" . $_POST['ajaxestuf'] . "' 
			order by
			 mundescricao asc";
    die($db->monta_combo("muncod", $sql, 'S', 'Selecione um Município', '', ''));
}

include APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';
#Foi alterado a função que cria a ABA e comentada para que esse formulário listagem não entre no grupo de abas.
//$db->cria_aba( $abacod_tela, $url, '' );
?>
<script type="text/javascript">
    function removerFiltro() {
        document.formulario.filtro.value = "";
        document.formulario.estuf.selectedIndex = 0;
        document.formulario.submit();
    }
</script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="./js/meajax.js" type="text/javascript"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tbody>
        <tr>
            <td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
                <form action="" method="POST" name="formulario">
                    <input type="hidden" name="pesquisa" value="1" />
                    <input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
                    <div style="float: left;">
                        <table width="100%" border="0" cellpadding="3" cellspacing="0">
                            <tr>
                                <td valign="bottom">
                                    Escola:
                                    <br/>
                                        <?php $escola = simec_htmlentities($_REQUEST['escola']); ?>
                                        <?= campo_texto('escola', 'N', 'S', '', 50, 200, '', '','','','',"class='limpa'"); ?>
                                </td>
                                <td valign="bottom">
                                    Código Escola:
                                    <br/>
                                    <?php $entcodent = simec_htmlentities($_REQUEST['entcodent']); ?>
                                    <?= campo_texto('entcodent', 'N', 'S', '', 30, 200, '', '','','','',"class='limpa'"); ?>
                                </td>
                                <td>
                                    <?php if (!checkPerfil(array(PERFIL_MINC_SEC_ESTADUAL, PERFIL_MINC_SEC_MUNICIPAL), false)): ?>
                                        Tipo:
                                        <br>
                                        <?php
                                        if ($_POST['tpcid'])
                                            $tpcid = $_POST['tpcid'];
                                        elseif ($_SESSION['maiseducacao']['filtro']['tpcid'])
                                            $tpcid = $_SESSION['maiseducacao']['filtro']['tpcid'];

                                        $sql = sprintf("SELECT
                                                                '1,3' AS codigo,
                                                                'Todas' AS descricao
                                                        UNION ALL
                                                        SELECT
                                                                '1' AS codigo,
                                                                'Estadual' AS descricao
                                                        UNION ALL
                                                        SELECT
                                                                '3' AS codigo,
                                                                'Municipal' AS descricao");
                                        $db->monta_combo("tpcid", $sql, 'S', 'Selecione...', '', '');
                                        ?>
                                    <?php endif; ?>
                                </td>
                                    <?
                                    // Recupera o perfil e testa se pode visualizar relatório geral consolidado.
                                    $usuPerfil = arrayPerfil();

                                    if (in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil) ||
                                        in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) ||
                                        in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) ||
                                        in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {

                                            $mostraBotaoRel = true;
                                    ?>
                                    <td rowspan="4" align="center" valign="middle" width="1000px">
                                        <div style="background-color:#f4f4f4; width:50%; border: 1px solid #a0a0a0;">
                                            <br />
                                            <b><font color="#000000">Para imprimir o Plano Consolidado clique no botão abaixo:</font></b><br /><br />
                                    <? if ($mostraBotaoRel) { ?>
                                                <input type="button" value="Relatório Geral Consolidado" onclick="abreRelGeralConsolidado();" />
                                    <? } else { ?>
                                                <input type="button" value="Relatório Geral Consolidado" onclick="alert('É necessário que todas as escolas estejam com a situação Finalizado.');" />
                                    <? } ?>
                                            <br /><br /><br />
                                        </div>
                                    </td>
                                <? } ?>
                            </tr>
                            <tr>
                                <td>
                                    Situação:
                                    <br>
                                        <?php
                                        $esdid = $_REQUEST['esdid'];
                                        $sql = "SELECT
                                                    esdid AS codigo,
                                                    esddsc AS descricao
                                                   FROM
                                                    workflow.estadodocumento et
                                                    INNER JOIN workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND
                                                           sisid = " . $_SESSION['sisid'] . "
                                                    WHERE et.tpdid = " . TPDID_MAIS_CULTURA . "
                                                   ORDER BY
                                                    esdordem;";

                                        $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '');
                                        ?>
                                    <script type="text/javascript">
                                        var esdid = document.getElementsByName("esdid");
                                        var option = document.createElement('option');

                                        option.text = "Não Iniciado";
                                        option.value = "naoiniciado";

                                        try {
                                            esdid[0].add(option, null); // standards compliant
                                        } catch (ex) {
                                            esdid[0].add(option); // IE only
                                        }

                                        if ("<?= $esdid ?>" == "naoiniciado") {
                                            esdid[0].options[5].selected = true;
                                        }
                                    </script>
                                </td>
                                <td valign="bottom">
                                    Estado
                                    <br/>
                                        <?php
                                        if ($_REQUEST['estuf'])
                                            $estuf = $_REQUEST['estuf'];
                                        elseif ($_SESSION['maiseducacao']['filtro']['estuf'])
                                            $estuf = $_SESSION['maiseducacao']['filtro']['estuf'];

                                        $sql = "select
                                                    e.estuf as codigo, e.estdescricao as descricao
                                                   from
                                                    territorios.estado e
                                                   order by
                                                    e.estdescricao asc";
                                        $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '');
                                        ?>
                                </td>
                                <td valign="bottom" id="municipio" style="visibility:<?= ($_REQUEST['estuf'] || $_SESSION['maiseducacao']['filtro']['estuf']) ? 'visible' : 'hidden' ?>;">
                                    Município
                                    <br/>
                                    <?
                                    if ($_REQUEST['estuf'] || $_SESSION['maiseducacao']['filtro']['estuf']) {
                                        if ($_REQUEST['muncod'])
                                            $muncod = $_REQUEST['muncod'];
                                        elseif ($_SESSION['maiseducacao']['filtro']['muncod'])
                                            $muncod = $_SESSION['maiseducacao']['filtro']['muncod'];

                                        $sql = "select
                                                        muncod as codigo,
                                                        mundescricao as descricao
                                                       from
                                                        territorios.municipio
                                                       where
                                                        estuf = '" . $estuf . "'
                                                       order by
                                                        mundescricao asc";
                                        $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '');
                                    }
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td valign="bottom">
                                    Modalidade de Ensino
                                    <br/>
                                    <?
                                    $modalidade = $_REQUEST['modalidade'];
                                    $sql = "SELECT
                                                    'F' AS codigo,
                                                    'Ensino Fundamental' AS descricao
                                            UNION ALL
                                            SELECT
                                                    'M' AS codigo,
                                                    'Ensino Médio' AS descricao";
                                    $db->monta_combo("modalidade", $sql, 'S', 'Selecione...', '', '');
                                    ?>
                                </td>
                                <td valign="bottom">
                                    Classificação
                                    <br/>
                                    <?
                                    $classificacao = $_REQUEST['classificacao'];
                                    $sql = "SELECT
                                                    'U' AS codigo,
                                                    'Urbana' AS descricao
                                            UNION ALL
                                            SELECT
                                                    'R' AS codigo,
                                                    'Rural' AS descricao
                                            UNION ALL
                                            SELECT
                                                    'A' AS codigo,
                                                    'Aberta' AS descricao";
                                    $db->monta_combo("classificacao", $sql, 'S', 'Selecione...', '', '');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td valign="bottom">
                                    Eixo Temático
                                    <br/>
                                    <?php
                                    $sql = "select
                                                    extid as codigo,
                                                    extdescricao as descricao
                                            from minc.mceeixotematico
                                            order by  extdescricao";

                                    $eixo = $_REQUEST['eixo'];
                                    $db->monta_combo("eixo", $sql, 'S', 'Selecione...', '', '');
                                    ?>
                                </td>
                                <td valign="bottom">
                                    &nbsp;
                                </td>
                            </tr>
                            <tr>
					            <td valign="bottom">
					            	Atividades
					            	<br>
					                <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="avaculturaafro" id="avacultura" <? if ($_POST['avaculturaafro'] == 't') echo 'checked = checked'; ?> value="t">  Cultura Afro Brasileira &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					                <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="avaculturaindigena" id="avacultura" <? if ($_POST['avaculturaindigena'] == 't') echo 'checked = checked'; ?> value="t"> Culturas Indígenas  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					                <input type="checkbox" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> name="avamusicanaescola" id="avacultura" <? if ($_POST['avamusicanaescola'] == 't') echo 'checked = checked'; ?> value="t"> Música nas Escolas &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					            </td>
					        </tr>
                            <tr>
                                <td valign="middle" id="aderiupst">
                                    Tipo de Escola:
                                    <ul style="margin: 0pt; padding: 0pt;">
                                        <li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="proemi" name="tipoescola" <?php echo ($_REQUEST['tipoescola'] == 'proemi') ? 'checked' : '' ?>/> ProEMI</li>
                                        <li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="pme" name="tipoescola" <?php echo ($_REQUEST['tipoescola'] == 'pme' && isset($_REQUEST['tipoescola']) ) ? 'checked' : '' ?>/> PME</li>
                                        <li style="margin: 0pt; width: 120px; list-style-type: none; float: left;"><input type="radio" value="ambos" name="tipoescola" <?php echo ($_REQUEST['tipoescola'] == 'ambos' && isset($_REQUEST['tipoescola'])) ? 'checked' : '' ?>/> ProEMI e PME</li>
                                    </ul>
                                </td>
                            </tr>
                            <tr>
                                <td valign="middle" id="aderiupst">Tipo de Busca:</td>
                            </tr>
                            <tr>
                                <td valign="middle" id="aderiupst">
                                    <input type="radio" name="cpfcnpj" id="cpf" value="1" <?php echo ($_REQUEST['cpfcnpj'] == '1') ? 'checked' : ''?> > CPF
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                    <input type="radio" name="cpfcnpj" id="cnpj" value="2" <?php echo ($_REQUEST['cpfcnpj'] == '2' && isset($_REQUEST['cpfcnpj']) ) ? 'checked' : '' ?> > CNPJ
                                </td>
                            </tr>
                            <?php




							if (!(in_array(PERFIL_MINC_SEC_ESTADUAL, $usuPerfil) || in_array(PERFIL_MINC_CADASTRADOR, $usuPerfil) || in_array(PERFIL_MINC_SEC_MUNICIPAL, $usuPerfil)) ) { ?>
                            <tr>
                                <td valign="bottom">
                                    Avaliação
                                    <br/>
                                        <?php
                                        $arrayAvaliacao = array(
                                            array('codigo' => 'a', 'descricao' => 'A'),
                                            array('codigo' => 'b', 'descricao' => 'B'),
                                            array('codigo' => 'c', 'descricao' => 'C')
                                        );

                                        $avaliacao = $_REQUEST['avaliacao'];

                                        $db->monta_combo("avaliacao", $arrayAvaliacao, 'S', 'Selecione...', '', '');
                                        ?>
                                </td>
                                <td valign="bottom">
                                    Avaliação por pontos
                                    <br/>
                                        <?php
                                        $arrayAvaliacao_pontos = array(
                                            array('codigo' => '0x', 'descricao' => '0'),
                                            array('codigo' => '1', 'descricao' => '1'),
                                            array('codigo' => '2', 'descricao' => '2'),
                                            array('codigo' => '3', 'descricao' => '3'),
                                            array('codigo' => '4', 'descricao' => '4')
                                        );

                                        $avaliacao_pontos = $_REQUEST['avaliacao_pontos'];

                                        $db->monta_combo("avaliacao_pontos", $arrayAvaliacao_pontos, 'S', 'Selecione...', '', '');
                                        ?>
                                </td>
                            </tr>
							<?php } ?>
                            <!--
                            <tr>
                                    <td valign="middle" id="usuativo">
                                            Usuário Ativo:
                                            <ul style="margin: 0pt; padding: 0pt;">
                                                    <li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="1" name="usuativo" <?php echo ($_REQUEST['usuativo'] == 1) ? 'checked' : '' ?>/> <label>Sim</label></li>
                                                    <li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="0" name="usuativo" <?php echo ( $_REQUEST['usuativo'] == 0 && isset($_REQUEST['usuativo']) ) ? 'checked' : '' ?>/> <label>Não</label></li>
                                            </ul>
                                    </td>
                            </tr>
                     <tr>
                                    <td valign="middle" id="aderiupst">
                                            Aderiu PST:
                                            <ul style="margin: 0pt; padding: 0pt;">
                                            <li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="S" name="aderiupst" <?php //echo ($_REQUEST['aderiupst'] == S) ? 'checked' : ''  ?>/> Sim</li>
                                            <li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="N" name="aderiupst" <?php //echo ($_REQUEST['aderiupst'] == N && isset($_REQUEST['aderiupst']) ) ? 'checked' : ''  ?>/> Não</li>
                                            <li style="margin: 0pt; width: 120px; list-style-type: none; float: left;"><input type="radio" value="null" name="aderiupst" <?php //echo ($_REQUEST['aderiupst'] == null && isset($_REQUEST['aderiupst'])) ? 'checked' : ''  ?>/> Não informado</li>
                                            </ul>
                                    </td>
                            </tr>  -->
                            <tr>
                                <td valign="middle" id="anoanterior">
                                    Escolas que participaram no ano anterior?
                                    <input type="checkbox" name="anoanterior" value="1" <?php echo ($_REQUEST['anoanterior'] == 1) ? 'checked' : '' ?>/>Sim
                                </td>
                            </tr>
                                <?php if (in_array(PERFIL_MINC_SUPER_USUARIO, $usuPerfil) || in_array(PERFIL_MINC_MEC, $usuPerfil)) { ?>
                                <tr>
                                    <td valign="middle" id="anoanterior">
                                        Mostrar escolas inativas?
                                        <input type="checkbox" name="status" value="1" <?php echo ($_REQUEST['status'] == 1) ? 'checked' : '' ?>/>Sim
                                    </td>
                                </tr>
                                <?php } ?>

                            <tr>
                                <td valign="middle">
                                    Maioria PBF?
                                    <!--<input type="checkbox" name="escolasPBF" value="1" <?php echo ($_REQUEST['escolasPBF'] == 1) ? 'checked' : '' ?>/>Sim-->
                                </td>
                            </tr>
                            <tr>
                                <td valign="middle" id="aderiupst">
                                    <input type="radio" name="escolasPBF" id="ckSim" value="T" <?php echo ($_REQUEST['escolasPBF'] == 'T') ? 'checked' : ''?> > Sim
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                    <input type="radio" name="escolasPBF" id="ckNao" value="F" <?php echo ($_REQUEST['escolasPBF'] == 'F') ? 'checked' : '' ?> > Não
                                </td>
                            </tr>

                            <tr>
                                <td valign="middle">
                                    Preencheu monitoramento?
                                </td>
                            </tr>
                            <tr>
                                <td valign="middle" id="aderiupst">
                                    <input type="radio" name="monitoramento" id="ckmSim" value="T" <?php echo ($_REQUEST['monitoramento'] == 'T') ? 'checked' : ''?> > Sim
                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                    <input type="radio" name="monitoramento" id="ckmNao" value="F" <?php echo ($_REQUEST['monitoramento'] == 'F') ? 'checked' : '' ?> > Não
                                </td>
                            </tr>

                            <tr>
                                <td><br/></td>
                            </tr>

                        </table>
                        <div style="float: left;">
                            <input type="hidden" name="requisicao" id="requisicao" value="">
                            <input type="button" name="btn_pesquisar" value="Pesquisar" onclick="return validaForm();"/>
                            <input type="button" name="btn_pesquisar" value="Limpar" onclick="location.href='minc.php?modulo=principal/lista&acao=A';">
                            <input type="button" name="" value="Gerar Excel" onclick="return geraExcel();"/>
                        </div>
                    </div>
                </form>
            </td>
        </tr>
    </tbody>
</table>
<?php 
if($_POST['pesquisa']){
	melista();
} 
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">


    d = document;


    /*
     * Faz requisição via ajax
     * Filtra o municipio, atravéz do parametro passado 'estuf'
     */
    function filtraTipo(estuf) {
        td = d.getElementById('municipio');
        select = d.getElementsByName('muncod')[0];

        /*
         * se estuf vazio
         * esconde o td do municipio e retorna
         */
        if (!estuf) {
            td.style.visibility = 'hidden';
            return;
        }

        // Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
        if (select) {
            select.disabled = true;
            select.options[0].text = 'Aguarde...';
            select.options[0].selected = true;
        }

        // Faz uma requisição ajax, passando o parametro 'ordid', via POST
        var req = new Ajax.Request('minc.php?modulo=principal/lista&acao=A', {
            method: 'post',
            parameters: '&ajaxestuf=' + estuf,
            onComplete: function(res)
            {
                var inner = 'Município<br/>';
                td.innerHTML = inner + res.responseText;
                td.style.visibility = 'visible';
            }
        });
    }


    function validaForm()
    {
        /*var p1 = document.formulario.preenchimento1;
         var p2 =  document.formulario.preenchimento2;

         if( isNaN( p1.value )  || isNaN( p2.value ) )
         {
         alert('Só é permitido números no filtro por preenchimento');
         p1.value = '';
         p2.value = '';
         return false;
         }*/
        window.document.getElementById('requisicao').value = '';
        document.formulario.submit();

    }

    function abreRelGeralConsolidado() {
        <? if ($_SESSION['exercicio'] == '2012') { ?>
                    //window.location = "pdeescola.php?modulo=meprincipal/meAdesao&acao=A";
        <? } else { ?>
                    //window.location = "pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A";
        <? } ?>
    }

    function inativarEscola(mceid)
    {
        if (confirm("Deseja inativar esta escola?"))
        {
            var req = new Ajax.Request('pdeescola.php?modulo=melista&acao=E', {
                method: 'post',
                parameters: '&ajaxinativar=1&mceid=' + mceid,
                onComplete: function(res)
                {
                    alert(res.responseText);
                    self.location.href = self.location.href;
                }
            });
        }
    }

    function ativarEscola(mceid)
    {
        if (confirm("Deseja ativar esta escola?"))
        {
            var req = new Ajax.Request('pdeescola.php?modulo=melista&acao=E', {
                method: 'post',
                parameters: '&ajaxativar=1&mceid=' + mceid,
                onComplete: function(res)
                {
                    alert(res.responseText);
                    self.location.href = self.location.href;
                }
            });
        }
    }

    function popupMapa(entid) {
        //window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
    }

    function geraExcel() {
        window.document.getElementById('requisicao').value = 'excel';
        document.formulario.submit();
    }

    /*limpa todos os tipos de campos do formulario*/
    $('#btnLimparTudo').click(function(){
        $('.limpa').val('');//limpa input text
        $(this.form).find('option').attr("selected","") ;//limpa combobox
        $("input:radio:checked").removeAttr("checked");//limpa radiobox
        $("input[type=checkbox]").removeAttr("checked");//limpar checkbox
    });
</script>