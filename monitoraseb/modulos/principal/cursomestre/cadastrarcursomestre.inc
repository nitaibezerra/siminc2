<?php

include_once APPRAIZ."monitoraseb/classes/Subacao.class.inc";
include APPRAIZ."monitoraseb/classes/CursoMestre.class.inc";

$oCursoMestre = new CursoMestre();
$sa = new SubAcao();

ini_set("memory_limit", "128M");
if($_REQUEST['excluirAnexo']){
        $oCursoMestre->excluirAnexo($_REQUEST['excluirAnexo']);
        die;
    }

    if($_REQUEST['abrirAnexo']){
        $oCursoMestre->abrirAnexo($_REQUEST['abrirAnexo']);
        die;
    }
if($_REQUEST['requisicaoAjax']){
    header('content-type: text/html; charset=ISO-8859-1');
    $_REQUEST['requisicaoAjax']();
    exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$titulo = "Cadastrar Curso Mestre";
monta_titulo( $titulo, '&nbsp;' );


if($_SERVER['REQUEST_METHOD']=="GET"){
    unset($_SESSION['listagemGradeCurricular']);
    unset($_SESSION['listagemEquipe']);
    unset($_SESSION['listagemOfertaVaga']);
    $permissoes = 'S';
    if($_REQUEST['cmtid'] && !empty($_REQUEST['cmtid'])){
        $cursoMestre = $oCursoMestre->carregaCursoMestrePorId($_REQUEST['cmtid']);
        extract($cursoMestre);
        $subacaoCoordenacao = $oCursoMestre->buscaSubacaoCoordenacao($scoid);
        $_SESSION['listagemGradeCurricular'] = $oCursoMestre->carregaGradeCurricularPorIdCursoMestre($cmtid);
        $_SESSION['listagemEquipe'] = $oCursoMestre->carregaEquipePorIdCursoMestre($cmtid);
        $_SESSION['listagemOfertaVaga'] = $oCursoMestre->carregaOfertaVagaPorIdCursoMestre($cmtid);
        $permissoes = $oCursoMestre->validarPermissaoGravar($_SESSION["usucpf"], $_REQUEST['cmtid']);
    }
}else if($_SERVER['REQUEST_METHOD']=="POST"){
    if ($_REQUEST['requisicao'] == 'salvar'){
        $oCursoMestre->salvarCursoMestre($_POST, $_FILES);
        die;
    }

}


?>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/monitoraseb.js"></script>
<script language="javascript" type="text/javascript">

    function validarFormularioPrincipal(){
        return validaFormulario();
    }

    function enviarFormulario(){
        document.formularioCadastro.submit();
    }

    function gravarCurso(){

        if(validaFormulario()){
            document.formularioCadastro.submit();
        }
    }

    function validaFormulario(){

        selectAllOptions(document.getElementById('funid'));
        var formCurso = document.formularioCadastro;

        if(!document.getElementById('usucpf').value) {
            document.getElementById('usucpf').focus();
            alert('O campo "Coordenador Técnico" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!document.getElementById('coonid').value) {
            document.getElementById('coonid').focus();
            alert('O campo "Coordenação" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(!document.getElementById('sbaid').value) {
            document.getElementById('sbaid').focus();
            alert('O campo "Subação" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtdsc'].value == "") {
            formCurso.elements['cmtdsc'].focus();
            alert('O campo "Descrição do Curso Mestre" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtementa'].value == "") {
            formCurso.elements['cmtementa'].focus();
            alert('O campo "Ementa do Curso Mestre" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtobjespecifico'].value == "") {
            formCurso.elements['cmtobjespecifico'].focus();
            alert('O campo "Descrição dos objetivos específicos" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['modalidade']) {
            var modalidade = formCurso.elements['modalidade'];
            var chk =false;
            for (var i=0;i<modalidade.length;i++){
                if(modalidade[i].checked==true){
                    if((modalidade[i].value=='d'||modalidade[i].value=='s') && (formCurso.elements['cmtcrghordistancia'].value == "0" ||formCurso.elements['cmtcrghordistancia'].value == "")) {
                        if(modalidade[i].value!='s'){
                            formCurso.elements['cmtcrghorpresencial'].value = '';
                        }
                        formCurso.elements['cmtcrghordistancia'].focus();
                        alert('O campo "Carga horária a distância" é obrigatório!');
                        document.getElementById('btnGravar').disabled=false;
                        return false;
                    }
                    if((modalidade[i].value=='p'||modalidade[i].value=='s') && (formCurso.elements['cmtcrghorpresencial'].value == "" || formCurso.elements['cmtcrghorpresencial'].value == "0")) {
                        if(modalidade[i].value!='s'){
                            formCurso.elements['cmtcrghordistancia'].value = '';
                        }
                        formCurso.elements['cmtcrghorpresencial'].focus();
                        alert('O campo "Carga horária presencial" é obrigatório!');
                        document.getElementById('btnGravar').disabled=false;
                        return false;
                    }
                    break;
                }
            }
        }

        if(formCurso.elements['cmtduracao'].value == "") {
            formCurso.elements['cmtduracao'].focus();
            alert('O campo "Duração" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtnumaluprojetado'].value == "") {
            formCurso.elements['cmtnumaluprojetado'].focus();
            alert('O campo "Número de alunos projetado" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtnumalumin'].value == "") {
            formCurso.elements['cmtnumalumin'].focus();
            alert('O campo "Número de alunos mínimo" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtnumalumax'].value == "") {
            formCurso.elements['cmtnumalumax'].focus();
            alert('O campo "Número de alunos máximo" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        var vlMin = parseInt(formCurso.elements['cmtnumalumin'].value);
        var vlMax = parseInt(formCurso.elements['cmtnumalumax'].value);
        if(vlMin > vlMax){
            alert('O campo "Número de alunos máximo" deve ser maior do que o campo "Número de alunos mínimo"!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(document.getElementById('funid').value=="") {
            document.getElementById('funid').focus();
            alert('O campo "Público-Alvo" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['cmtcertificacao'].value == "") {
            formCurso.elements['cmtcertificacao'].focus();
            alert('O campo "Descrição da certificação" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        if(formCurso.elements['ncmid']) {
            var nivelcurso = formCurso.elements['ncmid'];
            var chk =false;
            for (var i=0;i<nivelcurso.length;i++){
                if(nivelcurso[i].checked==true){
                    chk=true;
                    break;
                }
            }
            if(!chk){
                alert('O campo "Nível do curso" é obrigatório!');
                document.getElementById('btnGravar').disabled=false;
                return false;
            }
        }

        if(formCurso.elements['tgeid[]']) {
            var nivelcurso = formCurso.elements['tgeid[]'];
            var chk =false;
            for (var i=0;i<nivelcurso.length;i++){
                if(nivelcurso[i].checked==true){
                    chk=true;
                    break;
                }
            }
            if(!chk){
                alert('O campo "Nível de Escolaridade" é obrigatório!');
                document.getElementById('btnGravar').disabled=false;
                return false;
            }
        }

        if(formCurso.elements['cmtreqcursista'].value == "") {
            formCurso.elements['cmtreqcursista'].focus();
            alert('O campo "Requisitos do cursista" é obrigatório!');
            document.getElementById('btnGravar').disabled=false;
            return false;
        }

        var i = 0;
        while (formCurso.elements['arquivos_' + i]) {
            var arquivo = formCurso.elements['arquivos_' + i];
            var tipo = formCurso.elements['tidid_' + i];
            var descricao = formCurso.elements['arqdescricao_' + i];

            if (arquivo.value || tipo.value || descricao.value != "") {
                if (!(arquivo.value && tipo.value && descricao.value != "")) {
                    if (!arquivo.value) {
                        arquivo.focus();
                        alert('O campo "Documento" é obrigatório!');
                    } else if (!tipo.value) {
                        tipo.focus();
                        alert('O campo "Tipo do Documento" é obrigatório!');
                    } else if (descricao.value == "") {
                        descricao.focus();
                        alert('O campo "Descrição do Documento" é obrigatório!');
                    }
                    return false;
                }
            }
            i++;
        }

        return true;

    }

    function inserirNovosArquivos() {
        var tabela = document.getElementById('inserirarquivos');
        var line = tabela.insertRow(tabela.rows.length);
        line.insertCell(0).innerHTML = replaceAll(tabela.rows[1].cells[0].innerHTML, '_0', '_' + (tabela.rows.length - 2));
        line.insertCell(1).innerHTML = replaceAll(tabela.rows[1].cells[1].innerHTML, '_0', '_' + (tabela.rows.length - 2));
        line.insertCell(2).innerHTML = replaceAll(tabela.rows[1].cells[2].innerHTML, '_0', '_' + (tabela.rows.length - 2));
        line.insertCell(3).innerHTML = replaceAll(tabela.rows[1].cells[3].innerHTML, '_0', '_' + (tabela.rows.length - 2));
    }

    function limparAnexo(id){
        var tabela = document.getElementById('inserirarquivos');
        var line = tabela.rows[parseInt(id.substring(1)) + 1];
        line.cells[1].innerHTML = line.cells[1].innerHTML;
        document.formularioCadastro.elements['arqdescricao' + id].value = '';
        document.formularioCadastro.elements['tidid' + id].value = '';
    }

    function excluirAnexo(arqid){
        if (confirm('Deseja excluir o Documento?')) {
            location.href = window.location+'&excluirAnexo='+arqid;
        }
    }

    function abrirAnexo(arqid){
        location.href = window.location+'&abrirAnexo='+arqid;
    }

    function listarSubacao(coordenacaoId){
        mostraComboSubAcao(coordenacaoId,'mostraListaAcao','','','',document.getElementById('usucpf').value);

    }

    function mostraListaAcao(idsub){
        var cmtid=null;
        if(document.getElementById('cmtid') != null && document.getElementById('cmtid') != ''){
            cmtid = document.getElementById('cmtid').value
        }
        mostraListaAcoes(idsub,cmtid,  <?=($coonid && !empty($coonid))?$coonid:'null';?>, '<?=$permissoes?>');
    }

    function habilitaCampoModalidade(valor){
        if(valor){
            var formCurso = document.formularioCadastro;

            var div_p = document.getElementById('div_modalidade_p');
            var div_d = document.getElementById('div_modalidade_d');

            if(valor=='s'){
                div_p.style.display = 'block';
                div_d.style.display = 'block';
            }else if(valor=='p'){
                div_p.style.display = 'block';
                div_d.style.display = 'none';
                formCurso.elements['cmtcrghordistancia'].value = 0;
            }else{
                div_p.style.display = 'none';
                formCurso.elements['cmtcrghorpresencial'].value = 0;
                div_d.style.display = 'block';
            }
        }
        calculaTotal();
       }

    function voltar(){
        window.location = "?modulo=principal/cursomestre/consultarcursomestre&acao=A";
    }

    function excluirGradeCr(idGC) {
        mostraListaGradeCurricular(idGC);
    }

    function alterarGradeCr(idGC) {
        AbrirPopUp('?modulo=principal/cursomestre/popup/cadastrargradecurricular&acao=A&requisicao=alterar&nLinha='+idGC, 'Cadastrar Grade Curricular', '\'scrollbars=yes, width=700, height=260\'');
    }

    function excluirEquipe(ideq) {
        mostraListaEquipe(ideq,'<?=$permissoes?>');
    }

    function alterarEquipe(ideq) {
        AbrirPopUp('?modulo=principal/cursomestre/popup/cadastrarequipe&acao=A&requisicao=alterar&nLinha='+ideq, 'Cadastrar Equipe', '\'scrollbars=yes, width=700, height=300\'');
    }

    function excluirVaga(id) {
        mostraListaOfertaVaga(id,'<?=$permissoes?>');
    }

    function alterarVaga(id) {
        AbrirPopUp('?modulo=principal/cursomestre/popup/cadastrarofertavaga&acao=A&requisicao=alterar&nLinha='+id, 'Cadastrar Oferta de Vagas', '\'scrollbars=yes, width=350, height=130\'');
    }

    function calculaTotal(){
        var formCurso = document.formularioCadastro;
        var vldis = (formCurso.elements['cmtcrghordistancia'].value!='')? parseInt(formCurso.elements['cmtcrghordistancia'].value): 0;
        var vlpre = (formCurso.elements['cmtcrghorpresencial'].value!='')? parseInt(formCurso.elements['cmtcrghorpresencial'].value): 0;
        formCurso.elements['CHTotal'].value = vldis + vlpre;
    }

    function comparaMimMax(){
        var formCurso = document.formularioCadastro;
        var vlMin = parseInt(formCurso.elements['cmtnumalumin'].value);
        var vlMax = parseInt(formCurso.elements['cmtnumalumax'].value);
        if(vlMin > vlMax){
            alert('O campo "Número de alunos máximo" deve ser maior do que o campo "Número de alunos mínimo"!');
            formCurso.elements['cmtnumalumax'].focus();
        }
    }
</script>
<form name="formularioCadastro" id="formularioCadastro" method="post" enctype="multipart/form-data">
    <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td>
                <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Coordenador Técnico:</td>
                        <td colspan="2"><? $sa->comboCoordenadorTecnico(($usucpf ? $usucpf : $_SESSION['usucpf']), true, ($usucpf ? $usucpf : $_SESSION['usucpf']), true) ?></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Sigla / Coordenação:</td>
                        <td colspan="2">
                            <div id="coonid_on" style="display:none;">
                            </div>
                            <div id="coonid_off" style="color:#909090;">Selecione um Coordenador Técnico.</div>
                        </td>
                        <script type="text/javascript">
                            if(document.getElementById('usucpf') != null && document.getElementById('usucpf') != ''){
                                listar_coordenacao(document.getElementById('usucpf').value, <?=($coonid && !empty($coonid))?$coonid:'null';?> , <?=($sbaid && !empty($sbaid))?$sbaid:'null'?>, <?=($cmtid && !empty($cmtid))?$cmtid:'null'?>, '<?=$permissoes?>');
                            }
                        </script>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Subação (Programa Fantasia):</td>
                        <td colspan="2" align="left">
                            <div id="sbaid_on" style="display:none;" align="left">
                            </div>
                            <div id="sbaid_off" style="color:#909090;">Selecione uma Coordenação.</div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Ações:</td>
                        <td colspan="2" align="left">
                            <div id="acaid_on" style="display:none;" align="left">
                            </div>
                            <div id="acaid_off" style="color:#909090;">Selecione uma Subação.</div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Curso:</td>
                        <td colspan="2" align="left">
                            <?=campo_texto('cmtdsc','S',$permissoes,'Curso',50,50,'','');?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" style="vertical-align:top;">Ementa do Curso Mestre:</td>
                        <td colspan="2" align="left">
                            <?=campo_textarea('cmtementa','S', $permissoes, '', 54, 6, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" style="vertical-align:top;">Objetivo Específico:</td>
                        <td colspan="2" align="left">
                            <?=campo_textarea('cmtobjespecifico','S', $permissoes, '', 54, 6, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%">Modalidade:</td>
                        <td colspan="2" align="left">
                            <?=$oCursoMestre->campoModalidade($cmtcrghordistancia,$cmtcrghorpresencial, $permissoes);?>
                        </td>
                    </tr>
                    <tr >
                        <td class="SubtituloDireita" width="25%" style="vertical-align:top;">
                            Hora (s) / Aula
                        </td>
                        <td colspan="2" align="left">
                            <div id="div_modalidade_d" style="float: left;width: 190px; ">
                                A Distância:
                                <?=campo_texto('cmtcrghordistancia','S',$permissoes,'Carga Horária a distância',10,10,'##########','','left','',0,'','calculaTotal()');?>
                            </div>
                            <div id="div_modalidade_p">
                                Presencial:
                                <?=campo_texto('cmtcrghorpresencial','S',$permissoes,'Carga Horária a distância',10,10,'##########','','left','',0,'','calculaTotal()');?>
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Carga Horária Total:</td>
                        <td colspan="2" align="left">
                            <input type="text" name="CHTotal" id="CHTotal" value="<?=($cmtcrghorpresencial + $cmtcrghordistancia)?>" disabled="disabled" class="disabled" />
                        </td>
                    </tr>
                    <script>
                        var modCampo = document.formularioCadastro.modalidade;
                        for (var i=0;i<modCampo.length;i++){
                            if(modCampo[i].checked){
                                habilitaCampoModalidade(modCampo[i].value);
                                 break;
                            }
                        }
                    </script>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Duração:</td>
                        <td colspan="2" align="left">
                            <?=campo_texto('cmtduracao','S',$permissoes,'Duracao',10,10,'########','');?> Meses
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2" style="vertical-align:top;">Grade Curricular:</td>
                        <td colspan=2>
                            <?php if($permissoes=='S'){?>
                            <a href="#" onclick="AbrirPopUp('?modulo=principal/cursomestre/popup/cadastrargradecurricular&acao=A', 'Cadastrar Grade Curricular', '\'scrollbars=yes, width=700, height=260\'');"><img src="/imagens/gif_inclui.gif" border="0"/> Adicionar</a>
                            <?php }?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <div id="gcrid_on" style="display:none;" align="left">
                            </div>
                            <div id="gcrid_off" style="color:#909090;"></div>
                        </td>
                    </tr>
                    <script>
                        mostraListaGradeCurricular(null, '<?=$permissoes?>');
                    </script>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Número de Alunos por Turma:</td>
                        <td colspan="2" align="left">
                            <div style="float: left;width: 150px; ">
                                Projetado
                                <?=campo_texto('cmtnumaluprojetado','S',$permissoes,'Número projetado',10,10,'##########','');?> /
                            </div>
                            <div style="float: left;width: 150px; ">
                                Mínimo
                                <?=campo_texto('cmtnumalumin','S',$permissoes,'Número máximo',10,10,'##########','','left','',0,'','',null, 'comparaMimMax()', null );?>/
                            </div>
                            <div style="float: left;width: 150px; ">
                                Máximo
                                <?=campo_texto('cmtnumalumax','S',$permissoes,'Número máximo',10,10,'##########','','left','',0,'','',null, 'comparaMimMax()', null );?>
                            </div>

                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" style="vertical-align:top;">Público-Alvo:</td>
                        <td colspan="2">
                            <div id="funid_on" style="display:none;">
                            </div>
                            <div id="funid_off" style="color:#909090;">Selecione uma Subação.</div>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2" style="vertical-align:top;">Previsão de Ofertas de Vagas:</td>
                        <td colspan=2>
                            <?if($permissoes=="S"){?>
                            <a href="#" onclick="AbrirPopUp('?modulo=principal/cursomestre/popup/cadastrarofertavaga&acao=A', 'Cadastrar Oferta de Vagas', '\'scrollbars=yes, width=350, height=130\'');"><img src="/imagens/gif_inclui.gif" border="0"/> Adicionar</a>
                            <?}?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <div>
                                <div id="ovgid_on" style="display:none; float: left; width:841px" align="left" >
                                </div>
                                <div id="ovgid_off" style="color:#909090;"></div>
                            </div>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif" style="float: right">
                        </td>
                    </tr>
                    <script>
                        mostraListaOfertaVaga(null,'<?=$permissoes?>');
                    </script>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Certificação:</td>
                        <td colspan="2" align="left">
                            <?=campo_textarea('cmtcertificacao','S', $permissoes, 'Certificação', 54, 6, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" >Nível do Curso:</td>
                        <td colspan="2" align="left">
                            <?=$oCursoMestre->campoNivelCurso($permissoes);?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" style="vertical-align:top;">Requisitos para a Participação - Cursista:</td>
                        <td colspan="2" align="left">
                            <p>Nível de Escolaridade:</p>
                            <?=$oCursoMestre->campoNivelEscolaridade($cmtid, $permissoes);?>
                            <p>Descreva:</p>
                            <?=campo_textarea('cmtreqcursista','S', $permissoes, '', 54, 6, '','onkeyup=\'limitarTextoCampo(this, 500);\'','','','','',''); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2" style="vertical-align:top;">Equipe:</td>
                        <td colspan=2>
                            <?if($permissoes=="S"){?>
                            <a href="#" onclick="AbrirPopUp('?modulo=principal/cursomestre/popup/cadastrarequipe&acao=A', 'Cadastrar Equipe', '\'scrollbars=yes, width=700, height=300\'');"><img src="/imagens/gif_inclui.gif" border="0"/> Adicionar</a>
                            <?}?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <div id="ecmid_on" style="display:none;" align="left">
                            </div>
                            <div id="ecmid_off" style="color:#909090;"></div>
                        </td>
                    </tr>
                    <script>
                        mostraListaEquipe(null,'<?=$permissoes?>');
                    </script>
                    <tr>
                        <td class="SubtituloDireita" width="25%" rowspan="2">Documentos do Curso Mestre:</td>
                        <td colspan="2">
                            <? if($permissoes=='S') {?>
                            <a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a>
                            <table class="listagem" width="100%" id="inserirarquivos">
                                <tr>
                                    <th width="5%">Ação:</th>
                                    <th width="32%">Documento:</th>
                                    <th width="23%">Tipo Documento:</th>
                                    <th width="45%">Descrição Documento:</th>
                                </tr>
                                <tr>
                                    <td><center>
                                        <a onclick="javascript:limparAnexo('_0');" style="cursor: pointer;">
                                            <img border="0" title="Limpar" src="/imagens/borracha.gif">
                                        </a></center>
                                    </td>
                                    <td><input type="file" name="arquivos_0"></td>
                                    <td><?php $oCursoMestre->comboTipoDocumento(); ?></td>
                                    <td><?php echo campo_texto('arqdescricao_0', 'S', 'S', 'Descrição', 50, 255, '', '', '', '', 0, 'id="arqdescricao_0"' ); ?></td>
                                </tr>
                            </table>
                            <? }?>
                          </td>
                     </tr>
                      <tr>
                         <td colspan="2">
                            <div id="arqid_on" style="display:none;">
                            </div>
                            <div id="arqid_off" style="color:#909090;"></div>
                          </td>
                    </tr>
                    <?php if($cmtid){ ?>
                    <script>
                        atualizaDocumentoAnexo(null,'CursoMestre',<?=$cmtid?>, '<?=$permissoes?>');
                    </script>
                    <?php } ?>
                    <tr>
                        <th></th>
                        <th style="text-align: left;">
                            <input type="hidden" id="cmtid" name="cmtid" value="<?=$cmtid?>"/>
                            <input type="hidden" id="docid" name="docid" value="<?=$docid?>"/>
                            <input type="hidden" name="requisicao" value="salvar" />
                            <?php if($permissoes=='S'){?>
                            <input style="cursor: pointer;"  type="button" value="Salvar" name="btnGravar" onclick="gravarCurso();">
                            <?php }?>
                            <input style="cursor: pointer;"  type="button" value="Cancelar" name="btnCancelar" onclick="voltar();">
                        </th>
                    </tr>
                </table>
            </td>
            <td width="5%" valign="top">
                <? if($docid){
                    $dados_wf = array('cmtid' => $cmtid);
                    wf_desenhaBarraNavegacao($docid, $dados_wf);
                }
                ?>
            </td>
        </tr>
    </table>
</form>