<?PHP
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/funcoesspo_componentes.php";
$dataAtual = date('Y/m/d');
$dataLimite = date('2015/01/19');
$turma = buscarDadosTurma($_REQUEST['nteid']);

$aluno = buscarDadosAluno( (object) array('ateid' => $_REQUEST['ateid']) );
$alunojs['atecpf'] = $aluno['atecpf'] == '' ? '0' : $aluno['atecpf'];
if ($_REQUEST['requisicao'] == 'salvar') {
    $cpf = str_replace(array('-','.'), '', $_REQUEST['atecpf']);
    $dataNascimento = formata_data_sql($_REQUEST['atedatanasc']);
    
    # Regra - Verifica se o CPF ja foi cadastrado em um outro registro
    validarAlunoCPF((object) array(
        'notInAteid' => (int) $_REQUEST['ateid'],
        'cpf' => $cpf
    ));

    if(validarTurmaFechada($_REQUEST['nteid'], 'Cadastrar') === false){
        $js = "<script> window.location = 'eja.php?modulo=principal/aluno/listar&acao=C&nteid=".$turma['nteid']."'; </script>";
        echo $js;
        die;
    }

    if(empty($_REQUEST['ateid'])){
        if(validarTurmaQtdMaximaAlunos($_REQUEST['nteid']) === false){
            $js = "<script> window.location = 'eja.php?modulo=principal/aluno/listar&acao=C&nteid={$turma['nteid']}'; </script>";
            echo $js;
            die;
        }

        if( $_REQUEST['pupid'] != '' ){
            $sqlAno = "select nteexercicio from eja.novaturmaeja where nteid = '{$_REQUEST['nteid']}'";
            $anoAtual = $db->pegaUm($sqlAno);
            
            $sql = "
                INSERT INTO eja.alunoturmaeja (
                    nteid,
                    atecpf,
                    atenomedoaluno,
                    atedatanasc,
                    atenomedamae,
                    pupid,
                    usucpf,
                    atesexo,
                    ateexercicio
                )VALUES(
                    {$_REQUEST['nteid']},
                    '{$cpf}',
                    '{$_REQUEST['atenomedoaluno']}',
                    '{$dataNascimento}',
                    '{$_REQUEST['atenomedamae']}',
                    {$_REQUEST['pupid']},
                    '{$_SESSION['usucpf']}',
                    '{$_REQUEST['atesexo']}',
                    '{$anoAtual}'    
                ) RETURNING ateid;
            ";
        }

    } else {
        $sql = "
            UPDATE eja.alunoturmaeja
                SET nteid           = {$_REQUEST['nteid']},
                    pupid           = {$_REQUEST['pupid']},
                    atecpf          = '{$cpf}',
                    atenomedoaluno  = '{$_REQUEST['atenomedoaluno']}',
                    atenomedamae    = '{$_REQUEST['atenomedamae']}',
                    atedatanasc     = '{$dataNascimento}',
                    usucpf          = '{$_SESSION['usucpf']}',
                    atesexo         = '{$_REQUEST['atesexo']}'
                WHERE ateid = {$_REQUEST['ateid']} RETURNING ateid;
        ";
    }
    $ateid = $db->pegaUm($sql);

    if( $ateid > 0 ){
        $db->commit();
        $scriptRedirecionar = "<script> window.location = 'eja.php?modulo=principal/aluno/listar&acao=C&nteid={$turma['nteid']}'; </script>";
        echo $scriptRedirecionar;
        die;
    }else{
        $scriptRedirecionar = "<script> alert('Não foi possível executar a operação, tente novamente mais tarde! '); </script>";
        echo $scriptRedirecionar;
        die;
    }
   
}

?>

<div class="col-lg-12">

    <br />

    <div class="page-header">
        <h4 id="forms">Aluno - Cadastro</h4>
    </div>

    <div class="well">
        <?php echo mostrarDadosEscola($turma['pk_cod_entidade']); ?>
    </div>

    <br />

    <h4 id="forms">Turma <?php echo $turma['ntesequencial']; ?></h4>

    <br />

    <div class="well">
        <fieldset>
            <form id="formAluno" name="formAluno" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validarFormulario();">
                <input type="hidden" name="requisicao" id="requisicao" value="salvar">
                <input type="hidden" name="ateid" id="ateid" value="<?php echo $turma['nteid']; ?>">
                <input type="hidden" name="ateid" id="ateid" value="<?php echo $aluno['ateid']; ?>">
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        CPF:
                    </label>
                    <div class="col-lg-10">
                        <input type="text" maxlength="14" class="normal form-control" name="atecpf" id="atecpf" value="<?php echo $aluno['atecpf']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        NOME:
                    </label>
                    <div class="col-lg-10">
                        <input type="text" readonly="readonly" maxlength="255" class="normal form-control" name="atenomedoaluno" id="atenomedoaluno" value="<?php echo $aluno['atenomedoaluno']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        DATA DE NASCIMENTO:
                    </label>
                    <div class="col-lg-10">
                        <input type="text" readonly="readonly" maxlength="12" class="normal form-control" name="atedatanasc" id="atedatanasc" value="<?php echo $aluno['atedatanasc']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        NOME DA MÃE:
                    </label>
                    <div class="col-lg-10">
                        <input type="text" readonly="readonly" maxlength="255" class="normal form-control" name="atenomedamae" id="atenomedamae" value="<?php echo $aluno['atenomedamae']; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        PÚBLICO PRIORITÁRIO:
                    </label>
                    <div class="col-lg-10">
                        <?php
                            $pupid = $aluno['pupid'];
                            $sql = "
                                SELECT
                                    pupid AS codigo,
                                    pupdescricao AS descricao
                                FROM
                                    eja.publicoprioritario";
                            inputCombo("pupid", $sql, NULL, 'pupid')
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">
                        SEXO:
                    </label>
                    <div class="col-lg-10">
                        <?php
                            $atesexo = $aluno['atesexo'];
                            $arrSexo = Array(0 => Array('codigo' => 'F','descricao' => 'Feminino'  ) ,
                                             1 => Array('codigo' => 'M' ,'descricao' => 'Masculino' ));
                            inputCombo("atesexo", $arrSexo, $atesexo, "atesexo");
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-primary" id="btnVoltar" type="button" >Voltar</button>
                        <?php  if (strtotime($dataAtual) < strtotime($dataLimite) || $_SESSION['TipoUsuario']) {
                        ?>    
                        <button class="btn btn-success" id="btnCadastrar" type="submit">
                            <i class="glyphicon glyphicon-upload"></i>
                            Cadastrar
                        </button>
                        <?php }?>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<script type="text/javascript">

    jQuery( document ).ready(function() {
       jQuery('#btnVoltar').click(function(){
           voltar();
       });

       jQuery('#atecpf').mask("999.999.999-99");

       jQuery('#atecpf').blur(function(){
            buscarUsuarioReceitaFederal(jQuery(this).val());
       });

       jQuery('#atedatanasc').mask('99/99/9999');

    });

    function voltar(){
        window.location = 'eja.php?modulo=principal/aluno/listar&acao=C&nteid=<?php echo $turma['nteid']; ?>';
    }

    function validarFormulario() {
        if (jQuery('#atecpf').val() == "")
        {
            alert("O Campo CPF é de preenchimento obrigatório!");
            return false;
        }

        if (jQuery('#atenomedoaluno').val() == "")
        {
            alert("O Campo Nome é de preenchimento obrigatório!");
            return false;
        }

        if (jQuery('#pupid').val() == "")
        {
            alert("O Campo Público Prioritário é de preenchimento obrigatório!");
            return false;
        }
    }

    function buscarUsuarioReceitaFederal() {

        var usucpf = jQuery('#atecpf').val();
          
        usucpf = usucpf.replace('-','');
        usucpf = usucpf.replace('.','');
        usucpf = usucpf.replace('.','');
        
        if( usucpf != <?=$alunojs['atecpf']?> ){
                $.ajax({
                    url : '/includes/webservice/cpf.php',
                    type: 'post',
                    data: { ajaxCPF: usucpf },
                    dataType: "html",
                    success : function ( data ){
                        console.log(data);
                        pessoaFisica = formatarDadosPessoaFisicaReceita(data);

                        if(typeof(pessoaFisica['no_pessoa_rf']) == "undefined"){
                            alert('O CPF informado não foi encontrado na base de dados da Receita federal.');
                            return false;
                        } else {
                            dt_nascimento_rf = pessoaFisica['dt_nascimento_rf'];
                            dataNascimento = dt_nascimento_rf.substring(6, 8)
                                + '/' + dt_nascimento_rf.substring(4, 6)
                                + '/' + dt_nascimento_rf.substring(0, 4);
                            jQuery('#atenomedoaluno').val(pessoaFisica['no_pessoa_rf']);
                            jQuery('#atedatanasc').val(dataNascimento);
                            jQuery('#atenomedamae').val(pessoaFisica['no_mae_rf']);
                            jQuery('select[name=atesexo]').val(pessoaFisica['sg_sexo_rf']);
                            //jQuery('#atesexo').val(pessoaFisica['sg_sexo_rf']);
                            if( pessoaFisica['sg_sexo_rf'] == 'M' || pessoaFisica['sg_sexo_rf'] == 'F' ){
                                jQuery('#atesexo').val(pessoaFisica['sg_sexo_rf']).trigger('chosen:updated')
                            }

                        }
                    }

                });
            }
    }

//    function buscarDadosCpfReceita(cpf){
//		$.ajax({
//            url : '/includes/webservice/cpf.php',
//            type: 'post',
//            data: { ajaxCPF: cpf },
//            dataType: "html",
//            success : function ( data ){
//                return formatarDadosPessoaFisicaReceita(data);
//            }
//
//		});
//    }

    function formatarDadosPessoaFisicaReceita(text){
        var pessoaFisica = [];
        linhaItem = text.split('|');
        for (i=0; i < linhaItem.length; i++){
            colunaItem = linhaItem[i].split('#');
            pessoaFisica[colunaItem[0]] = colunaItem[1];
        }

        return pessoaFisica;
    }
</script>