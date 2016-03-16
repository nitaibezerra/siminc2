<?PHP
    if($_REQUEST['requisicao']){
        // ver($_REQUEST['requisicao'],d);
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Cadastro de Hospital' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/cad_hospital&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    
        $hspid = $_REQUEST['hspid'];
        $mntid = $_SESSION['maismedicomec']['mntid'];
        $_SESSION['maismedicomec']['hspid'] = $hspid;
    
    if( $hspid != '' ){
        $dados = buscarDadosHospital( $hspid );
        if( $dados['mntid'] != '' ){ 
            $habilita_mntid = 'N';
        }else{
            $habilita_mntid = 'S';
        }
        
        if($dados['rplsexo'] == 'M'){
            $checked_m = 'checked="checked"';
        }elseif($dados['rplsexo'] == 'F'){
            $checked_f = 'checked="checked"';
        }else{
            $checked_m = '';
            $checked_f = '';
        }
    }
    
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

    function abrirPopupSelecaoMantenedora(){
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_selecione_mantenedora&acao=A';        
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=650');
    }

    function atualizaComboMunicipioMantenedora( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipioMantenedora&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function buscarDadosRepresentanteLegal( cpf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosRepresentanteLegal&rplcpf="+cpf,
            success: function(resp){
                var dados = $.parseJSON(resp);
                //$LIMPA FORMULARIO. (APENAS CAMPOS REFERENTES AO REPRESENTANTE LEGAL)
                $('#rpldsc').val('');
                $('#rplrg').val('');
                $('input[name="rplsexo"]').attr('checked', false);
                $('#rplorgaoexprg').val('');
                $('#rplestuf').val('');
                $('#rpltelcomercial').val('');
                $('#rpltelcelular').val('');
                $('#rplemail').val('');

                $.each(dados, function(i, v){
                    $('#'+i).val(v);

                    $('#rplsexo_m').val('M');
                    $('#rplsexo_f').val('F');
                    if( i == 'rplsexo' && v == 'M' ){
                        $('#rplsexo_m').attr('checked', true);
                    }else{
                        $('#rplsexo_f').attr('checked', true);
                    }
                });
            }
        });
    }
    
    function buscarEndereceCEP( cep ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarEndereceCEP&cep="+cep,
            success: function(resp){
                var dados = $.parseJSON(resp);
                $('#hsplogradouro').val(dados.logradouro);
                $('#hspbairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function resetFromulario(){
        $('#selecionar').attr('disabled', false);
        $.each($("input[type=text], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'radio' ){
                $(v).attr('checked', false);
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }
        });
    }

    function salvarDadosHospital(){
        var erro;
        var campos = '';
        var formulario = document.formulario;

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosHospital');
            $('#formulario').submit();
        }
    }

    function verificarCNPJValido( cnpj ){
        var valido = validarCnpj( cnpj );

        if(!valido){
            alert( "CNPJ inválido! Favor informar um CNPJ válido!" );
            $('#hspcnpj').focus();
            return false;
        }
    }
    
    function verificarCPFValido( cpf ){
        var valido = validar_cpf( cpf );
        
        if(!valido){
            alert( "CPF inválido! Favor informar um CPF válido!" );
            $('#rplcpf').focus();
            return false;
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_hospital&acao=A';
        janela.focus();
    }
    
    function imprimirRecibo(){
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_hospital&acao=A';        
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=300,height=250');
        
    }

</script>

<?PHP
    $perfil = pegaPerfilGeral($_SESSION['usucpf']);

    if( in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
        $cpf = $_SESSION['usucpf'];
    }else{
        $cpf = $dados['srpnumcpf'];
    }
    if( $cpf != '' ){
       buscaUnidadesAssociadas( $cpf );
    }
?>


<?php $mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = ".$mntid;
$dados_mantenedora = $db->carregar($sql);

?>
   <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

        <tr>
            <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
            <td><?php echo $mntid ?></td>
        </tr>
        
          <tr>
            <td class ="SubTituloDireita" width="35%">Razão social</td>
            <td><?php echo $dados_mantenedora[0][mntrazaosocial];?></td>
        </tr>
   </table>
<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="rplid" name="rplid" value="<?=$dados['rplid'];?>"/>
    <input type="hidden" id="mntid" name="mntid" value="<?=$mntid;?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class="SubTituloDireita" width="25%"> Mantida:  </td>
            <td colspan="2">
                <?php
                    $mtdid = $dados['mtdid'];
                    $sql = "
                         SELECT  mtdid as codigo,
                                 mtddsc as descricao
                         FROM maismedicomec.mantida m
                         INNER JOIN gestaodocumentos.instituicaoensino ie ON ie.iesid = m.mtdid AND ie.mntid = {$mntid}
                         ORDER BY mtddsc
                     ";
                     $db->monta_combo('mtdid', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'mtdid', '', $mtdid, 'Mantida');
                ?>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Identificação </td>
        </tr>
        
        <?php if(!empty($dados['hspid'])){?>
        <tr>
            <td class="SubTituloDireita" width="25%"> Inscrição:  </td>
            <td colspan="2">
                <?PHP
                    echo $dados['hspid'];
                    
                ?>/<?php echo date("Y", strtotime($dados['hspdtinclusao'])); ?> 
            </td>
        </tr>
        <?php } ?>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPNJ: </td>
            <td colspan="2">
                <?PHP
                    $hspcnpj = $dados['hspcnpj'];
                    echo campo_texto('hspcnpj', 'S', 'S', 'CNPJ', 29, 18, '##.###.###/####-##', '', '', '', 0, 'id="hspcnpj"', '', $hspcnpj, 'verificarCNPJValido(this.value);', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Razão Social: </td>
            <td colspan="2">
                <?PHP
                    $hsprazaosocial = $dados['hsprazaosocial'];
                    echo campo_texto('hsprazaosocial', 'S', 'S', 'Razão Social', 89, 100, '', '', '', '', 0, 'id="hsprazaosocial"', '', $hsprazaosocial, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Sigla: </td>
            <td colspan="2">
                <?PHP
                    $hspsigla = $dados['hspsigla'];
                    echo campo_texto('hspsigla', 'N', 'S', 'Sigla', 29, 100, '', '', '', '', 0, 'id="hspsigla"', '', $hspsigla, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CNES: </td>
            <td colspan="2">
                <?PHP
                    $hspcnes = $dados['hspcnes'];
                    echo campo_texto('hspcnes', 'S', 'S', 'CNES', 29, 100, '', '', '', '', 0, "id='hspcnes'", "this.value=mascaraglobal('##########',this.value);", $hspcnes, "");
                ?>
            </td>
        </tr> 
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Endereço </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita"> CEP: </td>
            <td colspan="2">
               <?PHP
                    $hspcep = $dados['hspcep'];
                    echo campo_texto('hspcep', 'S', 'S', 'CEP', 29, 9, '#####-###', '', 'left', '', 0, 'id="hspcep"', '', $hspcep, 'buscarEndereceCEP(this.value)', '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Logradouro: </td>
            <td colspan="2">
               <?PHP
                    $hsplogradouro = $dados['hsplogradouro'];
                    echo campo_texto('hsplogradouro', 'S', 'S', 'Logradouro', 89, 100, '', '', 'left', '', 0, 'id="hsplogradouro"', '', $hsplogradouro, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Complemento: </td>
            <td colspan="2">
               <?PHP
                    $hspcomplementoend = $dados['hspcomplementoend'];
                    echo campo_texto('hspcomplementoend', 'N', 'S', 'Complemento', 89, 100, '', '', 'left', '', 0, 'id="hspcomplementoend"', '', $hspcomplementoend, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Bairro: </td>
            <td colspan="2">
               <?PHP
                    $hspbairro = $dados['hspbairro'];
                    echo campo_texto('hspbairro', 'S', 'S', 'Bairro', 89, 100, '', '', 'left', '', 0, 'id="hspbairro"', '', $hspbairro, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
                <?PHP
                    $estuf = $dados['hspestuf'];
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
                    $muncod = $dados['hspmuncod'];
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

                    $hsptelefone01 = $dados['hsptelefone01'];
                    echo campo_texto('hsptelefone01', 'N', 'S', 'Telefone 1', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="hsptelefone01"', '', $hsptelefone01, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 2: </td>
            <td colspan="2">
               <?PHP
                    $hsptelefone02 = $dados['hsptelefone02'];
                    echo campo_texto('hsptelefone02', 'N', 'S', 'Telefone 2', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="hsptelefone02"', '', $hsptelefone02, null, '', null);
                ?>

            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone 3: </td>
            <td colspan="2">
               <?PHP
                    $hsptelefone03 = $dados['hsptelefone03'];
                    echo campo_texto('hsptelefone03', 'N', 'S', 'Telefone 3', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="hsptelefone03"', '', $hsptelefone03, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> E-mail Institucional: </td>
            <td colspan="2">
                <?PHP
                    $hspemail = $dados['hspemail'];
                    echo campo_texto('hspemail', 'N', 'S', 'E-mail', 89, 100, '', '', '', '', 0, 'id="hspemail"', '', $hspemail, '', null);
                ?>
            </td>
        </tr>

        <!-- DIRIGENTE LEGAL -->
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Dirigente Legal </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    $rplcpf = $dados['rplcpf'];
                    echo campo_texto('rplcpf', 'S', 'S', 'CPF', 29, 18, '###.###.###-##', '', '', '', 0, 'id="rplcpf"', '', $rplcpf, 'buscarDadosRepresentanteLegal(this.value); verificarCPFValido(this.value);', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nome: </td>
            <td colspan="2">
                <?PHP
                    $rpldsc = $dados['rpldsc'];
                    echo campo_texto('rpldsc', 'S', 'S', 'Nome', 89, 100, '', '', '', '', 0, 'id="rpldsc"', '', $rpldsc, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Sexo: </td>
            <td colspan="2">
                <input type="radio" name="rplsexo" id="rplsexo_m" value="M" class="obrigatorio" <?=$checked_m;?>> Masculino
                <input type="radio" name="rplsexo" id="rplsexo_f" value="F" class="obrigatorio" <?=$checked_f;?>> Feminino
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> RG: </td>
            <td colspan="2">
               <?PHP
                    $rplrg = $dados['rplrg'];
                    echo campo_texto('rplrg', 'S', 'S', 'RG', 29, 20, '####################', '', 'left', '', 0, 'id="rplrg"', '', $rplrg, '', '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Órgão Expeditor: </td>
            <td colspan="2">
               <?PHP
                    $rplorgaoexprg = $dados['rplorgaoexprg'];
                    echo campo_texto('rplorgaoexprg', 'S', 'S', 'Órgão Expeditor', 29, 10, '', '', 'left', '', 0, 'id="rplorgaoexprg"', '', $rplorgaoexprg, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
                <?PHP
                    $rplestuf = $dados['rplestuf'];
                    $sql = "
                         SELECT  estuf as codigo,
                                 estuf as descricao
                         FROM territorios.estado
                         ORDER BY estuf
                     ";
                     $db->monta_combo('rplestuf', $sql, 'S', "Selecione...", '', '', '', 210, 'S', 'rplestuf', '', $rplestuf, 'Dirigente Legal - UF');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Comercial: </td>
            <td colspan="2">
               <?PHP
                    $textoDica = 'O telefone é composto por código de área (DDD) - Prefixo - Número do telefone. Ex: 61-2200-1111.';

                    $rpltelcomercial = $dados['rpltelcomercial'];
                    echo campo_texto('rpltelcomercial', 'S', 'S', 'Telefone Comercial', 29, 12, '##-####-####', '', 'left', '', 0, 'id="rpltelcomercial"', '', $rpltelcomercial, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Celular: </td>
            <td colspan="2">
               <?PHP
                    $rpltelcelular = $dados['rpltelcelular'];
                    echo campo_texto('rpltelcelular', 'S', 'S', 'Telefone Celular', 29, 12, '##-####-####', '', 'left', '', 0, 'id="rpltelcelular"', '', $rpltelcelular, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> E-mail Institucional: </td>
            <td colspan="2">
                <?PHP
                    $rplemail = $dados['rplemail'];
                    echo campo_texto('rplemail', 'S', 'S', 'E-mail', 89, 100, '', '', '', '', 0, 'id="rplemail"', '', $rplemail, '', null);
                ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosHospital();"/>
                <input type="button" id="cancela" name="cancela" value="Cancelar" onclick="resetFromulario();"/>
                
                <?php if(!empty($hspid)){?>
                <input type="button" value="Imprimir Nº de inscrição" onclick="imprimirRecibo();"/>
                <?php } ?>
                <?PHP
                    if( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ){
                ?>
                        <input type="button" id="voltar" name="voltar" value="voltar" onclick="voltarPaginaPrincipal();"/>
                <?PHP
                    }
                ?>
            </td>
        </tr>
    </table>
</form>