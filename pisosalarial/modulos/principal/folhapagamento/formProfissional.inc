<?php

verificaMuncod();

// Includes necessários
include_once APPRAIZ . 'includes/cabecalho.inc';
include_once APPPISO . 'classes/FolhaPagamento.class.inc';
include_once APPPISO . 'classes/FolhaPagamentoProfissionais.class.inc';
require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "www/includes/webservice/cpf.php";

// Objetos
$obFolhaPagamento = new FolhaPagamento();
$obFolhaProfissionais = new FolhaPagamentoProfissionais();

// Paramentros folha de pagamento
$arParams = array('muncod' => $_SESSION['piso']['muncod'], 'flpanoreferencia' => $_REQUEST['anoref'], 'flpmesreferencia' => $_REQUEST['mesref']);
$flpid    = $obFolhaPagamento->recuperarFolhaPagamento($arParams, true);

// Verifica se a folha existe, se não cria
if(empty($flpid)){

    $arDados = array('muncod'           => $_SESSION['piso']['muncod'],
                     'flpanoreferencia' => $_REQUEST['anoref'],
                     'flpmesreferencia' => $_REQUEST['mesref']);

    $flpid = $obFolhaPagamento->gerarFolhaPagamento($arDados);
}

// Verifica se não exite e gera folha de pagamento
$obFolhaPagamento->gerarFolhaPagamento($arParams);
?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">
    jQuery.noConflict();
</script>

<?php

// Salva profissional, Editar e Novo
if($_REQUEST['requisicao'] == 'salvar'){

    $retorno = $obFolhaProfissionais->salvarFolhaProfissional($_REQUEST);

    if($retorno == 'existe'){

        // Alerta que existe
        echo "<script>
                alert('Este CPF já se encontra cadastrado.');
              </script>";

    }else{

        // Redireciona para lista de profissionais
        echo "<script>
                alert('Operação realizada com sucesso.');
                jQuery(document).ready(function(){
                    jQuery('#formVoltar').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A');
                    jQuery('#formVoltar').submit();
                });
              </script>";

        echo '<form id="formVoltar" method="post" action="">
                <input type="hidden" name="anoref" value="'.$_REQUEST['anoref'].'">
                <input type="hidden" name="mesref" value="'.$_REQUEST['mesref'].'">
                <input type="hidden" name="muncod" value="'.$_SESSION['piso']['muncod'].'" />
              </form>';

        exit;

    }
}

// Recupera dados para editar profissional
if($_REQUEST['requisicao'] == 'editar'){

    // Recupera profissional e cria variáveis
    $rsEditar = $obFolhaProfissionais->recuperarPorId($_REQUEST['fppid']);
    extract($rsEditar);

    if(!empty($entcodent)){
        // Recupera escola e cria variaveis para popular o campo_popup
        $entcodent = $obFolhaProfissionais->recuperarEscolaCampoPopup($entcodent);
    }

    if(!empty($entcodent) || $lotid == 1){
           // Mostra combo autocomplete das escolas
           echo "<script>
                   jQuery(document).ready(function(){
                       jQuery('#cmb_lotacao').show();
                       jQuery('#dsc_lotacao').hide();
                       jQuery('input[name=fppdesclotacao]').removeClass('required');
                       jQuery('select[name=entcodent]').addClass('required');
                   });
                 </script>";

    }else{
           // Mostra descrição da lotação
           echo "<script>
                   jQuery(document).ready(function(){
                       jQuery('input[name=fppdesclotacao]').addClass('required');
                       jQuery('select[name=entcodent]').removeClass('required');
                   });
                 </script>";
    }
}

// Monta abas
print '<br/>';
$abacod_tela = '57365';
$arMnuid = array(ABA_FP_DUPLICAR_MES, ABA_FP_IMPORTAR_ARQUIVO, ABA_FP_LISTAR_PROFIS);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// Monta cabeçalho
monta_titulo('Incluir Profissional', "Folha de Pagamento");
montaCabecalho();
?>

<script type="text/javascript">

    jQuery(document).ready(function(){

        jQuery("a:contains('Lista Profissionais')").click(function(){
            jQuery('#formVoltar').attr('action', this.href);
            jQuery('#formVoltar').submit();
            this.href = 'javascript:void(0)';
        })

        // Habilita jQuery Validation
        jQuery("#formulario").validate({
            rules: {
                fppcpf: "required",
                fppnome: "required",
                carid: "required",
                forid: "required",
                vicid: "required",
                sitid: "required",
                fppcargahoraria: "required",
                lotid: "required",
                fppsalbase: "required",
                fppadigrat: "required"
            },
            errorPlacement: function(error, element){

                 error.insertAfter(element);

                 if(jQuery('#entcodent').next().attr('class') == 'error'){

                    var label = jQuery('#entcodent').next();
                    jQuery('#entcodent').next().remove();
                    jQuery('#cmbtd').append(label);

                 }

            }
        });

       jQuery('select[name=carid], select[name=lotid]').change(function(){

            if(jQuery('select[name=carid]').val() == '68' && jQuery('select[name=lotid]').val() == '1'){
                jQuery('#entcodent').addClass('required');

            }else{
                jQuery('#entcodent').removeClass('required');
            }

        });


        // Verifica evento IE
        var evt = jQuery.browser.msie ? "change" : "blur";

        // Verifica CPF e busca NOME
        jQuery('input.classcpf').live('change',function(){
            if( !validar_cpf( jQuery(this).val()  ) ){
                alert( "CPF inválido!\nFavor informar um cpf válido!" );
                jQuery(this).val('');
                return false;
            }
            var comp  = new dCPF();

            comp.buscarDados( jQuery(this).val() );
            if (comp.dados.no_pessoa_rf != ''){
                jQuery('#fppnome').val( comp.dados.no_pessoa_rf );
            }
        });

        // Verifica carga horaria
        jQuery('#fppcargahoraria').live(evt, function(){
            if(this.value != ''){
                if(this.value > 60){
                    alert('A carga horária não pode ser maior que 60 horas!');
                    this.value = '';
                    this.focus();
                    return false;
                }
            }
        });

        // Submete formulario
        jQuery('#formulario').submit(function(){
            jQuery('input[name=requisicao]').val('salvar');
            jQuery('#formulario').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/formProfissional&acao=A');
        });

        // Mostra oculta combos das escolas
        jQuery('select[name=lotid]').change(function(){
            if(this.value == 1){
                jQuery('#cmb_lotacao').show();
                jQuery('#dsc_lotacao').hide();
//                jQuery('input[name=fppdesclotacao]').val('required');
                jQuery('input[name=fppdesclotacao]').val('');
                jQuery('input[name=fppdesclotacao]').removeClass('required');
            }else{
                jQuery('#cmb_lotacao').hide();
                jQuery('#dsc_lotacao').show();
                jQuery('select[name=entcodent]').val('');
//                jQuery('input[name=fppdesclotacao]').addClass('required');
            }

        });

        // Botão voltar
        jQuery('.voltar').click(function(){
            jQuery('#formVoltar').attr('action', 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A');
            jQuery('#formVoltar').submit();
        });

        // Plugin auto complete
        jQuery('.campo_popup_autocomplete').autocomplete("/geral/campopopup.php?nome=entcodent&autocomplete=1", {
            cacheLength:50,
            max:50,
            width: 440,
            scrollHeight: 220,
            delay: 1000,
            selectFirst: true,
            autoFill: false
        });

        jQuery('.campo_popup_autocomplete').result(function(event, data, formatted) {
             if (data) {

                // Extract the data values
                var campoHidden = jQuery(this).attr('id').replace('_dsc','');

                var descricao = data[0];
                var id = data[1];

                if( id == '' ){
                    jQuery(this).val('');
                    return false;
                }

                jQuery('#'+campoHidden).val( id );
                jQuery('#'+campoHidden+'_retorno').val( descricao );
            }
        });

        jQuery('.campo_popup_autocomplete').blur(function (){
            jQuery(this).val(jQuery(this).val());
            var campoHidden = jQuery(this).attr('id').replace('_dsc','');
            if( jQuery(this).val() != '' && jQuery('#'+campoHidden+'_retorno').val() != '' ){
                if( jQuery(this).val() != jQuery('#'+campoHidden+'_retorno').val() ){
                    jQuery(this).val('Selecione...')
                    jQuery('#'+campoHidden).val('')
                    jQuery('#'+campoHidden+'_retorno').val('')
                }
            }else{
                jQuery(this).val('Selecione...')
            }
        });

        jQuery('select[name=carid]').change(function(){

            if(this.value == '142' || this.value == '143' || this.value == '144'){
                jQuery('input[name=caroutros]').attr('readonly', false);
                jQuery('input[name=caroutros]').removeClass('disabled');
                jQuery('input[name=caroutros]').addClass('normal');
            }else{
                jQuery('input[name=caroutros]').attr('readonly', true);
                jQuery('input[name=caroutros]').addClass('disabled');
                jQuery('input[name=caroutros]').removeClass('normal');
                jQuery('input[name=caroutros]').val('');
            }
            
        });

        jQuery('select[name=carid], select[name=lotid]').change(function(){

            if(jQuery('select[name=carid] option:selected').html().indexOf('Professor') == 0 ||  jQuery('select[name=carid] option:selected').html().indexOf('Outros - Professor') == 0 && jQuery('select[name=lotid]').val() == 1){
                
                if(!jQuery('select[name=etpid]').hasClass('required')){
                    
                    jQuery('select[name=etpid]').addClass('required');
                    jQuery('select[name=etpid]').parent().append('<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
                    jQuery('select[name=fppzona]').addClass('required');
                    jQuery('select[name=fppzona]').parent().append('<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
                    jQuery('select[name=sebid]').addClass('required');
                    jQuery('select[name=sebid]').parent().append('<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
                }

            }else{
                
                jQuery('select[name=etpid]').removeClass('required');
                jQuery('select[name=etpid]').next().remove();
                jQuery('select[name=fppzona]').removeClass('required');
                jQuery('select[name=fppzona]').next().remove();
                jQuery('select[name=sebid]').removeClass('required');
                jQuery('select[name=sebid]').next().remove();

            }
            
            console.log(jQuery('select[name=etpid]').hasClass('required'));

        });

        jQuery('.ver_opcoes_cargo').click(function(){
            jQuery('#cmb_cargo').toggle();
            if(jQuery('#cmb_cargo').css('display') != 'none'){
                jQuery(this).html('Ocultar cargos');
            }else{
                jQuery(this).html('Mostrar cargos');
            }
        });

        <?php if( in_array($carid, array(142,143,144)) ): ?>
            jQuery('input[name=caroutros]').attr('readonly', false);
            jQuery('input[name=caroutros]').removeClass('disabled');
            jQuery('input[name=caroutros]').addClass('normal');
        <?php endif;?>

    });

    // Calcula remuneração
    function calculaRenumeracao(){

        var flpadigrat = $('fppadigrat').value;
        var flpsalbase = $('fppsalbase').value;
        var result = 0;

        flpadigrat = flpadigrat.replace(/\./gi,"");
        flpadigrat = flpadigrat.replace(/\,/gi,".");

        flpsalbase = flpsalbase.replace(/\./gi,"");
        flpsalbase = flpsalbase.replace(/\,/gi,".");

        result = parseFloat(flpsalbase) + parseFloat(flpadigrat);
        $('fpprenumeracao').value =  mascaraglobal('###.###.###.###,##', result.toFixed(2));
    }
</script>
<form name="formulario" id="formulario" method="post">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="flpid" id="flpid" value="<?=$flpid; ?>">
    <input type="hidden" name="anoref" value="<?php echo $_REQUEST['anoref'] ?>">
    <input type="hidden" name="mesref" value="<?php echo $_REQUEST['mesref'] ?>">
    <table class="tabela" align="center" bgcolor="#f5f5f5" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td class="SubtituloCentro" colspan="2">Dados do Profissional</td>
        </tr>
        <tr>
            <td class="SubtituloDireita">CPF</td>
            <td><?=campo_texto('fppcpf', 'S', $habilita, 'CPF', 20, 14, '###.###.###-##', '', '', '', 0, "id=fppcpf class='normal classcpf'" ); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Nome</td>
            <td><?=campo_texto('fppnome', 'N', 'N', 'Nome', 80, 100, '', '', '', '', 0, 'id=fppnome' ); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Cargo</td>
            <td>
                <?
                $sql = "SELECT carid as codigo, cardsc as descricao FROM pisosalarial.cargo WHERE carstatus = 'A' ORDER BY cardsc";
                $db->monta_combo("carid", $sql, 'S', 'Selecione...', '', '', '', '265', 'S', '', '', '', 'Cargo' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Descrição do Cargo</td>
            <td>
                <?=campo_texto('caroutros', 'N', 'N', 'Descrição do cargo', 80, 100, '', '', '', '', 0, 'id=caroutros' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Formação</td>
            <td><?
            $sql = "SELECT forid as codigo, fordsc as descricao FROM pisosalarial.formacao WHERE forstatus = 'A'";
            $db->monta_combo("forid", $sql, 'S', 'Selecione...', '', '', '', '265', 'S', '', '', '', 'Formação' );
            ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Vínculo</td>
            <td><?
            $sql = "SELECT vicid as codigo, vicdsc as descricao FROM pisosalarial.vinculo WHERE vicstatus = 'A' ORDER BY vicdsc";
            $db->monta_combo("vicid", $sql, 'S', 'Selecione...', '', '', '', '150', 'S', '', '', '', 'Vínculo' );
            ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Situação</td>
            <td><?
            $sql = "SELECT sitid as codigo, sitdsc as descricao FROM pisosalarial.situacao WHERE sitstatus = 'A' ORDER BY sitdsc";
            $db->monta_combo("sitid", $sql, 'S', 'Selecione...', '', '', '', '150', 'S', '', '', '', 'Situação' );
            ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Carga horária</td>
            <td><?=campo_texto('fppcargahoraria', 'S', 'S', 'Carga horária', 6, 20, '##', '', '', '', 0, 'id=fppcargahoraria' ); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo de Lotação</td>
            <td><?
            $sql = "SELECT lotid as codigo, lotdsc as descricao FROM pisosalarial.lotacao WHERE lotstatus = 'A' ORDER BY lotdsc";
            $db->monta_combo("lotid", $sql, 'S', 'Selecione...', '', '', '', '265', 'S', '', '', '', 'Tipo de Lotação' );
            ?></td>
        </tr>
        <tr id="cmb_lotacao" style="display:none;">
            <td class="SubtituloDireita">Descrição da Lotação</td>
            <td id="cmbtd">
                <?

                $sql = "SELECT
                            e.entcodent as codigo,
                            entnome as descricao
                        FROM entidade.entidade e
                        INNER JOIN entidade.endereco ed ON ed.entid = e.entid
                        INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid
                        INNER JOIN entidade.funcao fu ON fu.funid = f.funid
                        WHERE e.entstatus = 'A'
                        AND e.entcodent IS NOT NULL
                        AND f.funid = 3
                        AND fu.funstatus = 'A'
                        AND ed.muncod = '{$_SESSION['piso']['muncod']}'
                        AND e.tpcid = 3
                        ORDER BY entnome";

//              $db->monta_combo("entcodent", $sql, 'S', 'Selecione...', '', '', '', '265', 'S', '', '', '', 'Descrição da Lotação' );

                $arColunas = array(array("codigo" => "entcodent",
                                        "descricao" => "<b>Código:</b>","string" => "1"),
                                  array("codigo" => "entnome",
                                        "descricao" => "<b>Descrição:</b>","string" => "1"));

                $arAutocomplete = array('class'=>'campo_popup_autocomplete',
                                        'whereAuto'=>'entnome');

                campo_popup('entcodent',$sql,'Selecione','','400x800','60', $arColunas, 1, false, '', '', '', $arAutocomplete);
                ?>

            </td>
        </tr>
        <tr id="dsc_lotacao">
            <td class="SubtituloDireita">Descrição da Lotação</td>
            <td><?=campo_texto('fppdesclotacao', 'N', 'S', 'Descrição da Lotação', 80, 100, '', '', '', '', 0, 'id=fppdesclotacao' ); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Etapa</td>
            <td><?
            $sql = "SELECT etpid as codigo, etpdsc as descricao FROM pisosalarial.etapa WHERE etpstatus = 'A'";
            $db->monta_combo("etpid", $sql, 'S', 'Selecione...', '', '', '', '265', 'N', '', '', '', 'Etapa' );
            ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Zona</td>
            <td><?
            $sql = array(
                        array('codigo' => 'ZR', 'descricao' => 'Zona Rural'),
                        array('codigo' => 'ZU', 'descricao' => 'Zona Urbana')
                        );
            $db->monta_combo("fppzona", $sql, 'S', 'Selecione...', '', '', '', '150', 'N', '', '', '', 'Zona' );
            ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Segmento da Educação Básica</td>
            <td><?
            $sql = "SELECT sebid as codigo, sebdsc as descricao FROM pisosalarial.segedubasica WHERE sebstatus = 'A'";
            $db->monta_combo("sebid", $sql, 'S', 'Selecione...', '', '', '', '265', 'N', '', '', '', 'Segmento da Educação Básica' );
            ?></td>
        </tr>
        <tr>
            <?php
            if(!empty($fppadigrat) && !empty($fppsalbase)){

                $calculo = $fppsalbase+$fppadigrat;
                $fpprenumeracao = number_format($calculo, 2, ',', '.');

                $fppsalbase = number_format($fppsalbase, 2, ',', '.');
                $fppadigrat = number_format($fppadigrat, 2, ',', '.');
            }
            ?>
            <td class="SubtituloDireita">Salário Base(R$)</td>
            <td><?=campo_texto('fppsalbase', 'S', 'S', 'Salário Base(R$)', 30, 20, '[###.]###,##', '', '', '', 0, 'id=fppsalbase' ); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Adicionais e Gratificações(R$)</td>
            <td><?=campo_texto('fppadigrat', 'S', 'S', 'Adicionais e Gratificações(R$)', 30, 20, '[###.]###,##', '', '', '', 0, 'id=fppadigrat', '', '', 'calculaRenumeracao();' ); ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Remuneração(Sal. Base + Adic.)</td>
            <td>

                <?=campo_texto('fpprenumeracao', 'N', 'N', 'Remuneração(Sal. Base + Adic.)', 30, 40, '[###.]###,##', '', '', '', 0, 'id=fpprenumeracao' ); ?>
            </td>
        </tr>
        <tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
                <?php if(!empty($_REQUEST['fppid'])): ?>
                    <input type="hidden" name="fppid" value="<?php echo $_REQUEST['fppid'] ?>" />
                <?php endif; ?>
                <input type="submit" id="btInserir" value="Salvar" />
                <input type="button" value="Cancelar" class="voltar" />
            </td>
        </tr>
    </table>
</form>
<form id="formVoltar" method="post" action="">
    <input type="hidden" name="anoref" value="<?php echo $_REQUEST['anoref'] ?>">
    <input type="hidden" name="mesref" value="<?php echo $_REQUEST['mesref'] ?>">
    <input type="hidden" name="muncod" value="<?php echo $_SESSION['piso']['muncod'] ?>" />
</form>