<?PHP
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
}

if ($_SESSION['maismedicomec']['mntid'] == '') {
    $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Ocorreu um problema, é necessário o preenchimento dos dados da Mantida. Tente novamente!");
}

verificaMantenedora();

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Mais Médicos - MEC', 'Cadastro da Mantida');

$abacod_tela = ABA_CADASTRO_MANTENEDORA;
$url = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantida_candidata&acao=A';
$parametros = '';

$db->cria_aba($abacod_tela, $url, $parametros);


if ($mnmid == '') {
    $mnmid = $_REQUEST['mnmid'];
}
if (!empty($_REQUEST['mnmid'])) {
    $_SESSION['maismedicomec']['mnmid'] = $_REQUEST['mnmid'];
} else {
    unset($_SESSION['maismedicomec']['mnmid']);
}


if ($mnmid != '') {
    $dados = buscarDadosMantidaCandidata($mnmid);
    
    if ($dados['mnmid'] != '') {
        $habilita_mnmid = 'N';
    } else {
        $habilita_mnmid = 'S';
    }

    if ($dados['mtdimovelsit'] == 'P') {
        $checked_p = 'checked="checked"';
    } elseif ($dados['mtdimovelsit'] == 'A') {
        $checked_a = 'checked="checked"';
    } elseif ($dados['mtdimovelsit'] == 'O') {
        $checked_o = 'checked="checked"';
    } else {
        $checked_p = '';
        $checked_a = '';
        $checked_o = '';
    }

    #FACULDADE
    if ($dados['mtdfaculdade'] == 't') {
        $ck_s_mtdfaculdade = 'checked="checked"';
    } elseif ($dados['mtdfaculdade'] == 'f') {
        $ck_n_mtdfaculdade = 'checked="checked"';
    } else {
        $ck_s_mtdfaculdade = '';
        $ck_n_mtdfaculdade = '';
    }
    #CENTRO UNIVERSITARIO
    if ($dados['mtdcentrouniversitario'] == 't') {
        $ck_s_mtdcentrouniversitario = 'checked="checked"';
    } elseif ($dados['mtdcentrouniversitario'] == 'f') {
        $ck_n_mtdcentrouniversitario = 'checked="checked"';
    } else {
        $ck_s_mtdcentrouniversitario = '';
        $ck_n_mtdcentrouniversitario = '';
    }
    #UNIVERSIDADE
    if ($dados['mtduniversidade'] == 't') {
        $ck_s_mtduniversidade = 'checked="checked"';
    } elseif ($dados['mtduniversidade'] == 'f') {
        $ck_n_mtduniversidade = 'checked="checked"';
    } else {
        $ck_s_mtduniversidade = '';
        $ck_n_mtduniversidade = '';
    }

    #FILANTROPIA
    if ($dados['mtdfilantropica'] == "t") {
        $ck_s_mtdfilantropica = 'checked="checked"';
    } elseif ($dados['mtdfilantropica'] == "f") {
        $ck_n_mtdfilantropica = 'checked="checked"';
    } else {
        $ck_s_mtdfilantropica = '';
        $ck_n_mtdfilantropica = '';
    }

    #COMUNITÁRIA
    if ($dados['mtdcomunitaria'] == "t") {
        $ck_s_mtdcomunitaria = 'checked="checked"';
    } elseif ($dados['mtdcomunitaria'] == "f") {
        $ck_n_mtdcomunitaria = 'checked="checked"';
    } else {
        $ck_s_mtdcomunitaria = '';
        $ck_n_mtdcomunitaria = '';
    }

    #CONFESSIONAL
    if ($dados['mtdconfessional'] == "t") {
        $ck_s_mtdconfessional = 'checked="checked"';
    } elseif ($dados['mtdconfessional'] == "f") {
        $ck_n_mtdconfessional = 'checked="checked"';
    } else {
        $ck_s_mtdconfessional = '';
        $ck_n_mtdconfessional = '';
    }

    #AUTORIZAÇÃO
    //$ck_s_mtdautfuncionamento $ck_s_mtdcredenciamento
    if ($dados['mtdautfuncionamento'] == "t") {
        $ck_s_mtdautfuncionamento = 'checked="checked"';
    } elseif ($dados['mtdautfuncionamento'] == "f") {
        $ck_n_mtdautfuncionamento = 'checked="checked"';
    } else {
        $ck_s_mtdautfuncionamento = '';
        $ck_n_mtdautfuncionamento = '';
    }
    #CREDENCIAMENTO
    if ($dados['mtdcredenciamento'] == "t") {
        $ck_s_mtdcredenciamento = 'checked="checked"';
    } elseif ($dados['mtdcredenciamento'] == "f") {
        $ck_n_mtdcredenciamento = 'checked="checked"';
    } else {
        $ck_s_mtdcredenciamento = '';
        $ck_n_mtdcredenciamento = '';
    }
    
	#PRIVADA
    if( $dados['mtdprivada'] == "t"){
    	$ck_s_mtdprivada = 'checked="checked"';
    }elseif( $dados['mtdprivada'] == "f" ){
    	$ck_n_mtdprivada = 'checked="checked"';
    }else{
    	$ck_s_mtdprivada = '';
    	$ck_n_mtdprivada = '';
    }
}
$_SESSION['maismedicomec']['mnmid'] = $mnmid;
$perfil = pegaPerfilGeral();
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">

	//alert("ATENÇÃO \n\n1- Indicar Mantida para autorização de curso de medicina\n ou \n 2- Credenciar Nova Mantida para autorização de curso de medicina. \nNo caso 1, preencher a Aba Mantida Indicada.\n No caso 2, preencher a Aba Credenciar nova mantida.");

    $1_11(document).ready(function() {
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11(window).unload(
                $1_11('.chosen-select').chosen()
                );
    });

    function abrirPopupSelecaoMantida() {
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_selecione_mantida&acao=A';
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=650');
    }

    function atualizaComboMunicipioMantenedora(estuf) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=atualizaComboMunicipioMantenedora&estuf=" + estuf,
            async: false,
            success: function(resp) {
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function buscarDadosMantidaGD(codigo) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=buscarDadosMantidaGD&codigo=" + codigo,
            success: function(resp) {
                var dados = $.parseJSON(resp);

                $('#mtdid').val(dados.iesid);
                $('#mtddsc').val(dados.iesdsc);
                $('#mntsigla').val(dados.iessigla);
                $('#mtdcep').val(dados.iescep);
                $('#mtdsite').val(dados.iessite);

                var cep = trim(dados.iescep);
                buscarEndereceCEP(cep.replace("-", ""));
            }
        });
    }

    function buscarEndereceCEP(cep) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=buscarEndereceCEP&cep=" + cep,
            success: function(resp) {
                var dados = $.parseJSON(resp);

                $('#mtdlogradouro').val(dados.logradouro);
                $('#mtdbairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function buscarDadosEditarMantidaCandidata(mnmid) {

        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantida_candidata&acao=A&mnmid=' + mnmid;
    }

    function excluirDadosMantidaCandidata(mnmid) {
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if (confirma) {
            $.ajax({
                type: "POST",
                url: window.location,
                data: "requisicao=excluirDadosMantidaCandidata&mnmid=" + mnmid,
                success: function(resp) {
                    if (trim(resp) == 'OK') {
                        alert('Operação Realizada com sucesso!');
                        location.reload(true);
                    }
                }
            });
        }
    }

    function resetFromulario() {
        $("#mtdid").attr('readonly', false);
        $("#mtdid").removeClass('disabled');
        $("#mtdid").addClass('obrigatorio normal');

        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i, v) {
            if ($(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox') {
                $(v).val('');
            }
            if ($(this).attr('type') == 'radio') {
                $(v).attr('checked', false);
            }
            if ($(this).attr('type') == 'checkbox') {
                $(v).attr('checked', false);
            }
        });
    }
    
    
    function resetFromulario2() {
       location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantida_candidata&acao=A';
    }

    function salvarDadosMatidaCandidata() {
        var erro;
        var radio_entro;
        var check_entro;
        var campos = '';

        $.each($(".obrigatorio"), function(i, v) {
            if (($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox')) {
                if ($(v).val() == '') {
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            } else
            if ($(this).attr('type') == 'radio') {
                var name = $(this).attr('name');
                var value = $(this).attr('value');
                var radio_box = $('input:radio[name=' + name + ']:checked');

                if (!radio_box.val() && (name != radio_entro)) {
                    erro = 1;
                    radio_entro = name;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            } else
            if ($(this).attr('type') == 'checkbox') {
                var name = $(this).attr('name');
                var value = $(this).attr('value');
                var check_box = $('input:checkbox[name=' + name + ']:checked');

                if (!check_box.val() && (name != check_entro)) {
                    erro = 1;
                    check_entro = name;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }
        });

        if (erro > 0) {
            alert('Existem "campos obrigatorios" vazios! São eles: \n' + campos);
            return false;
        }

        if (!erro) {
            $('#requisicao').val('salvarDadosMatidaCandidata');
            $('#formulario').submit();
        }
    }

    function voltarPaginaPrincipal() {
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A';
        janela.focus();
    }

    function imprimirRecibo() {
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_mantida&acao=A&tipo=2';
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=300,height=250');

    }
    
    function pesquisarMantida(){
        
        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantida_candidata&acao=A&mun='+document.formulario.mantida_muncod.value+'&nome='+document.formulario.mtddsc.value+'&sigla='+document.formulario.mntsigla.value;
    }
    
</script>

<?php
$mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = " . $mntid;
$dados_mantenedora = $db->carregar($sql);


$sql1 = "
        SELECT  count(m.mtdid) as quantidade
        FROM maismedicomec.mantidacandidata AS m

        JOIN maismedicomec.mantidamunicipiocandidata AS mm ON mm.mtdid = m.mtdid

          JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
         JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
         JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}

        WHERE mtdstatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}";

if(in_array(PERFIL_HOSPITAL, $perfil)){            
            $sql1 .= " and pe.pflcod = ".PERFIL_HOSPITAL;
            $sql1 .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
        if(in_array(PERFIL_IES, $perfil)){            
            $sql1 .= " and pe.pflcod = ".PERFIL_IES;
            $sql1 .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
    

  $RS = $db->recuperar($sql1,$res);
   $RS['quantidade'];


?>
<table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

    <tr>
        <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
        <td><?php echo $mntid ?></td>
    </tr>

    <tr>
        <td class ="SubTituloDireita" width="35%">Razão social</td>
        <td><?php echo $dados_mantenedora[0][mntrazaosocial]; ?></td>
    </tr>
</table>


<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="mm_mtdid" name="mm_mtdid" value="<?= $dados['mm_mtdid']; ?>"/>
    <input type="hidden" id="mnmid" name="mnmid" value="<?= $dados['mnmid'] ?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
		<?php
		if (
		        in_array(PERFIL_HOSPITAL, $perfil) === false || (in_array(PERFIL_HOSPITAL, $perfil) === true && in_array(PERFIL_SUPER_USUARIO, $perfil) === true)) {
		    ?>
		            <tr>
		                <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Município para qual concorre </td>
		            </tr>
		
		            <tr>
		                <td class ="SubTituloDireita"> Município: </td>
		                <td id="td_combo_mantida_muncod" colspan="2">
						    <?PHP
						    $mantida_muncod = $dados['mantida_muncod'] ? $dados['mantida_muncod'] : $_REQUEST['mun'];
						    $sql = "
						                            SELECT  mun.muncod AS codigo,
						                                    mundescricao || ' - ' || estuf AS descricao
						                            FROM territorios.municipio mun
						                            JOIN maismedicomec.municipioies munies on substr(mun.muncod, 1, 6) = munies.muncod 
						                            ORDER BY descricao
						                        ";
						    $db->monta_combo('mantida_muncod', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'mantida_muncod', '', $mantida_muncod, 'Município', '', 'chosen-select');
						    ?>
		                </td>
		            </tr>
		<?php } ?>

		<tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Prioridade </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Prioridade: </td>
            <td colspan="2">
                <?PHP
                
                	if($dados['mnmid']) $andmnmid = " and mnmid not in ({$dados['mnmid']}) ";
                	$sqlp = "select mnmprioridade from maismedicomec.mantidamunicipiocandidata where mntid = {$_SESSION['maismedicomec']['mntid']} {$andmnmid}
                			 UNION
                			 select mnmprioridade from maismedicomec.mantidamunicipio where mntid = {$_SESSION['maismedicomec']['mntid']}";
                	$verificaPrioridade = $db->carregarColuna($sqlp);
                	
                
                    $mnmprioridade = $dados['mnmprioridade'];
                    $arraymnmprioridade = array();
                    for($i=1;$i<=5;$i++){
                    	if(!in_array("{$i}", $verificaPrioridade)){
                    		array_push($arraymnmprioridade, array("codigo" => $i, "descricao" => $i));
                    	}
                    }
                    $db->monta_combo('mnmprioridade', $arraymnmprioridade, 'S', "Selecione...", '', '', '', 300, 'S', 'mnmprioridade', '', $mnmprioridade, 'Prioridade', '', '');
                ?>
            </td>
        </tr>
        <tr>
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Identificação </td>
        </tr>
        <? if($dados['mnmid']){ ?>
        <tr>
            <td class="SubTituloDireita" width="25%"> Código Mantida: </td>
            <td colspan="1" width="17%">
				<?PHP
				$mtdid = $dados['mtdid'];
				echo campo_texto('mtdid', 'N', "N", 'Código Mantida', 29, 10, '##########', '', '', '', 0, 'id="mtdid"', '', $mtdid, '', null);
				?>
            </td>
            <td>
            	<!-- 
                <input type="button" id="selecionar" name="selecionar" value="Aguardar Credenciamento MEC">
                 -->
            </td>
        </tr>
        <?}?>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nome da Mantida: </td>
            <td colspan="2">
				<?PHP
				$mtddsc = $dados['mtddsc'] ? $dados['mtddsc'] : $_REQUEST['nome'];
				echo campo_texto('mtddsc', 'S', 'S', 'Nome da Mantida', 89, 100, '', '', '', '', 0, 'id="mtddsc"', '', $mtddsc, '', null);
				?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Sigla: </td>
            <td colspan="2">
				<?PHP
				$mntsigla = $dados['mntsigla'] ? $dados['mntsigla'] : $_REQUEST['sigla'];
				echo campo_texto('mntsigla', 'S', 'S', 'Sigla', 60, 20, '', '', '', '', 0, 'id="mntsigla"', '', $mntsigla, '', null);
				?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Disponibilidade do imóvel: </td>
            <td colspan="2">
                <input type="radio" name="mtdimovelsit" id="mtdimovelsit_p" value="P" class="obrigatorio" title="Disponibilidade do imóvel" <?= $checked_p; ?>> Próprio
                <input type="radio" name="mtdimovelsit" id="mtdimovelsit_a" value="A" class="obrigatorio" title="Disponibilidade do imóvel" <?= $checked_a; ?>> Alugado
                <input type="radio" name="mtdimovelsit" id="mtdimovelsit_O" value="O" class="obrigatorio" title="Disponibilidade do imóvel" <?= $checked_o; ?>> Outro
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Endereço </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita"> CEP: </td>
            <td colspan="2">
<?PHP
$mtdcep = $dados['mtdcep'];
echo campo_texto('mtdcep', 'S', 'S', 'CEP', 29, 9, '#####-###', '', 'left', '', 0, 'id="mtdcep"', '', $mtdcep, 'buscarEndereceCEP(this.value)', '', null);
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Logradouro: </td>
            <td colspan="2">
<?PHP
$mtdlogradouro = $dados['mtdlogradouro'];
echo campo_texto('mtdlogradouro', 'S', 'S', 'Logradouro', 89, 100, '', '', 'left', '', 0, 'id="mtdlogradouro"', '', $mtdlogradouro, null, '', null);
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Complemento: </td>
            <td colspan="2">
<?PHP
$mtdcomplementoend = $dados['mtdcomplementoend'];
echo campo_texto('mtdcomplementoend', 'N', 'S', 'Complemento', 89, 100, '', '', 'left', '', 0, 'id="mtdcomplementoend"', '', $mtdcomplementoend, null, '', null);
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Bairro: </td>
            <td colspan="2">
<?PHP
$mtdbairro = $dados['mtdbairro'];
echo campo_texto('mtdbairro', 'S', 'S', 'Bairro', 89, 100, '', '', 'left', '', 0, 'id="mtdbairro"', '', $mtdbairro, null, '', null);
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
<?PHP
$estuf = $dados['estuf'];
$sql = "
                         SELECT  estuf as codigo,
                                 estuf as descricao
                         FROM territorios.estado
                         ORDER BY estuf
                     ";
$db->monta_combo('estuf', $sql, 'S', "Selecione...", 'atualizaComboMunicipioMantenedora', '', '', 300, 'S', 'estuf', '', $estuf, 'UF');
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Município: </td>
            <td id="td_combo_municipio" colspan="2">
<?PHP
$muncod = $dados['muncod'];
$sql = "
                        SELECT  muncod AS codigo,
                                mundescricao AS descricao
                        FROM territorios.municipio
                        ORDER BY descricao
                    ";
$db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'muncod', '', $muncod, 'Município');
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 1: </td>
            <td colspan="2">
<?PHP
$textoDica = 'O telefone é composto por código de área (DDD) - Prefixo - Número do telefone. Ex: 61-2200-1111.';

$mtdtelefone01 = $dados['mtdtelefone01'];
echo campo_texto('mtdtelefone01', 'N', 'S', 'Telefone 1', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="mtdtelefone01"', '', $mtdtelefone01, null, '', null);
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 2: </td>
            <td colspan="2">
<?PHP
$mtdtelefone02 = $dados['mtdtelefone02'];
echo campo_texto('mtdtelefone02', 'N', 'S', 'Telefone 2', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="mtdtelefone02"', '', $mtdtelefone02, null, '', null);
?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 3: </td>
            <td colspan="2">
<?PHP
$mtdtelefone03 = $dados['mtdtelefone03'];
echo campo_texto('mtdtelefone03', 'N', 'S', 'Telefone 3', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="mtdtelefone03"', '', $mtdtelefone03, null, '', null);
?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Site: </td>
            <td colspan="2">
<?PHP
$mtdsite = $dados['mtdsite'];
echo campo_texto('mtdsite', 'S', 'S', 'Site', 60, 100, '', '', '', '', 0, 'id="mtdsite"', '', $mtdsite, '', null);
?>
            </td>
        </tr>



        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Categoria Administrativa </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Filantrópica: </td>
            <td colspan="2">
                <input type="radio" name="mtdfilantropica" id="mtdfilantropica_s" value="t" class="obrigatorio categoria_administrativa" title="Filantrópica" <?=$ck_s_mtdfilantropica;?>> SIM
                <input type="radio" name="mtdfilantropica" id="mtdfilantropica_n" value="f" class="obrigatorio categoria_administrativa" title="Filantrópica" <?=$ck_n_mtdfilantropica;?>> NÃO
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Comunitária: </td>
            <td colspan="2">
                <input type="radio" name="mtdcomunitaria" id="mtdcomunitaria_s" value="t" class="obrigatorio categoria_administrativa" title="Comunitária" <?=$ck_s_mtdcomunitaria;?>> SIM
                <input type="radio" name="mtdcomunitaria" id="mtdcomunitaria_n" value="f" class="obrigatorio categoria_administrativa" title="Comunitária" <?=$ck_n_mtdcomunitaria;?>> NÃO
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Confessional: </td>
            <td colspan="2">
                <input type="radio" name="mtdconfessional" id="mtdconfessional_s" value="t" class="obrigatorio categoria_administrativa" title="Confessional" <?=$ck_s_mtdconfessional;?>> SIM
                <input type="radio" name="mtdconfessional" id="mtdconfessional_n" value="f" class="obrigatorio categoria_administrativa" title="Confessional" <?=$ck_n_mtdconfessional;?>> NÃO
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Privada com fins lucrativos: </td>
            <td colspan="2">
                <input type="radio" name="mtdprivada" id="mtdprivada_s" value="t" class="obrigatorio categoria_administrativa" title="Privada com fins lucrativos:" <?=$ck_s_mtdprivada;?>> SIM
                <input type="radio" name="mtdprivada" id="mtdprivada_n" value="f" class="obrigatorio categoria_administrativa" title="Privada com fins lucrativos:" <?=$ck_n_mtdprivada;?>> NÃO
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>



    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                
                <?PHP
            		$perfil = pegaPerfilGeral();
                	$docid = criaDocidMantenedora( $_SESSION['maismedicomec']['mntid'] );
                	$esdid = pegaEstadoAtualWorkflow( $docid );
                
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
	                    <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosMatidaCandidata();"/>
                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
                <?PHP
                    }elseif( in_array(PERFIL_IES, $perfil) && ($esdid == WF_EM_PREENCHIMENTO_IES || $esdid == WF_EM_AJUSTE_IES) ){
                    	if(date('Ymd') < 20150124){
	                    	?>
	                    	<input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosMatidaCandidata();"/>
	                		<input type="button" id="novo" name="novo" value="Novo" onclick="resetFromulario2();"/>
	                    	<?
						}
                    	
                    }
                ?>
                
                
				<?php if (!empty($_GET['mnmid'])) { ?>
				     <input type="button" id="imprimir" value="Imprimir Nº de inscrição" onclick="imprimirRecibo();"/>
                <?php } ?>

                <input type="button" id="cancela" name="cancela" value="Cancelar" onclick="resetFromulario2();"/> 
                <input type="button" id="voltar" name="voltar" value="voltar" onclick="voltarPaginaPrincipal();"/>
				
				<input type="button" id="perquisar" name="perquisar" value="perquisar" onclick="pesquisarMantida();"/>
            </td>
        </tr>
    </table>
</form>

<script>
        
    jQuery('.categoria_administrativa').click(function(e){
        if( jQuery(this).val() == 't' ){

            if( jQuery(this).attr('id') != 'mtdfilantropica_s' ){
                jQuery('#mtdfilantropica_n')           .attr('checked', 'checked');
            }

            if( jQuery(this).attr('id') != 'mtdcomunitaria_s' ){
                jQuery('#mtdcomunitaria_n') .attr('checked', 'checked');
            }

            if( jQuery(this).attr('id') != 'mtdconfessional_s' ){
                jQuery('#mtdconfessional_n')        .attr('checked', 'checked');
            }
            if( jQuery(this).attr('id') != 'mtdprivada_s' ){
                jQuery('#mtdprivada_n')        .attr('checked', 'checked');
            }
            
        }
        
        
        if(jQuery('#mtdfilantropica_s').is(':checked') == false 
           && jQuery('#mtdcomunitaria_s').is(':checked') == false 
           && jQuery('#mtdconfessional_s').is(':checked') == false
           && jQuery('#mtdprivada_s').is(':checked') == false ){
        	alert("É necessário selecionar pelo menos um SIM!");
        	jQuery('#mtdfilantropica_s').attr('checked', 'checked');
        }
        
     })
</script>

<?PHP
	$acao = "
		        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarMantidaCandidata('||mm.mnmid||');\" title=\"Editar Mantida\" >
		        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDadosMantidaCandidata('||mm.mnmid||');\" title=\"Editar Mantida\" >
		    ";

	if( in_array(PERFIL_IES, $perfil) && ($esdid != WF_EM_PREENCHIMENTO_IES && $esdid != WF_EM_AJUSTE_IES) || (date('Ymd') > 20150123) ){
   		$acao = "
		        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosEditarMantidaCandidata('||mm.mnmid||');\" title=\"Editar Mantida\" >
			    ";
    }

	//filtro
    if($_REQUEST['mun']){
    	$and .= " and mm.muncod = '{$_REQUEST['mun']}' ";
    }
	if($_REQUEST['nome']){
    	$and .= " and mtddsc ilike '%{$_REQUEST['nome']}%' ";
    }
	if($_REQUEST['sigla']){
    	$and .= " and mntsigla ilike '%{$_REQUEST['sigla']}%' ";
    }

$sql = "
        SELECT  '{$acao}' as acao,
                mtddsc,
                mntsigla,
                mundescricao,
                mnmprioridade,
                mtdtelefone01,
                mtdsite,
                pe.pfldsc as pfldsc
        FROM maismedicomec.mantidacandidata AS m

        JOIN maismedicomec.mantidamunicipiocandidata AS mm ON mm.mtdid = m.mtdid

        left JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
        JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
        JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}

        WHERE mtdstatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}
        
        $and
        
        ";

if (in_array(PERFIL_HOSPITAL, $perfil)) {
    $sql .= " and pe.pflcod = " . PERFIL_HOSPITAL;
}

if (in_array(PERFIL_IES, $perfil)) {
    $sql .= " and pe.pflcod = " . PERFIL_IES . " ";
    $sql .= "           ";
}

$sql .= " ORDER BY 2";
//$cabecalho = array("Ação", "Nome", "Sigla", "Município", 'Telefone 01', 'Site', "Perfil do responsável");
$cabecalho = array("Ação", "Nome", "Sigla", "Município", "Prioridade", 'Telefone 01', 'Site', "Perfil do responsável");
$alinhamento = Array('center', '', '', '', 'center', '', '', '', '', '', '');
$tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
//alert("ATENÇÃO \n\n1- Indicar Mantida para autorização de curso de medicina\n ou \n 2- Credenciar Nova Mantida para autorização de curso de medicina. \nNo caso 1, preencher a Aba Mantida Indicada.\n No caso 2, preencher a Aba Credenciar nova mantida.");
/*** INICIO SHOW MODAL ***/

var countModal = 1;

function montaShowModal() {
	var campoTextArea = ''+
		'<p align=\'justify\'><CENTER><FONT COLOR=RED><B>ATENÇÃO</B></FONT></CENTER>'+
		'<br />1- Indicar Mantida para autorização de curso de medicina.'+
		'<CENTER><FONT COLOR=RED><B>ou</B></FONT></CENTER>'+
		'2- Credenciar Nova Mantida para autorização de curso de medicina.'+
		'<br /><br />No caso 1, preencher a Aba Mantida Indicada.'+
		'<br />No caso 2, preencher a Aba Credenciar nova mantida.</p>';
	var alertaDisplay = '<div class="titulo_box" >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(520,180);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

montaShowModal();

</script>
