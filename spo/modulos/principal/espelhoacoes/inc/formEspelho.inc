<?
$requisicao = !isset($dados['arqid']) ? 'anexo' : 'atualizaAnexo';
if($permissaoAcesso): ?>
<section class="well">
    <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validaCampo();">
        <input type="hidden" name="requisicao" value="<?=$requisicao ?>">
        <input type="hidden" name="arqid" id="arqid" value="<?=$dados['arqid']; ?>">
        <section class="form-group">
            <label for="file" class="col-lg-2 control-label">Anexar Arquivos:</label>
            <section class="col-lg-9">
                <?php if(!$dados['arqid']): ?>
                    <input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
                <?php else: ?>
                    <p class="form-control-static">
                        <a href="" class="buttonDownload" title="Download"
                            data-request="&requisicao=download&arqid=<?=$dados['arqid']?>">
                            <?=$dados['arqnome']; ?>.<?=$dados['arqextensao']; ?>
                        </a>
                    </p>
                <?php endif; ?>
            </section>
        </section>
        <section class="form-group">
            <label for="nomeArquivo" for="nomeArquivo" class="col-lg-2 control-label">Nome Arquivo:</label>
            <section class="col-lg-10">
                <? inputTexto('nomeArquivo', $dados['angdsc'], 'nomeArquivo', '250',false); ?>
            </section>
        </section>
        <section class="form-group">
            <label for="tipo" class="col-lg-2 control-label">Tipo:</label>
            <section class="col-lg-10">
                <?
                $opcoes = array('Superior' => 'SU', 'Básica' => 'BA', 'Gestão' => 'GE', 'Tecnológica' => 'TE', 'Outros' => 'OU');
                inputChoices('tipo', $opcoes, $dados['arptipo'], 'tipo');
                ?>
            </section>
        </section>
        <section class="form-group">
            <section class="col-lg-10">
                <button class="btn btn-warning" type="reset" onclick="window.location = window.location;"><span class="glyphicon glyphicon-refresh"></span> Limpar</button>
                <button class="btn btn-success" id="inserir" type="submit">
                    <i class="glyphicon glyphicon-upload"></i>
                    Salvar
                </button>
            </section>
        </section>
    </form>
</section>

<script type="text/javascript">
    function validaCampo(){

        if(document.formPrincipal.file.value==""){
            bootbox.alert("Favor selecionar um arquivo!");
            return false;
        }

        if(document.formPrincipal.nomeArquivo.value==""){
            bootbox.alert("O Campo nome é obrigatório!");
            return false;
        }

        //validando preenchimento do radiobutton
        var radios = document.formPrincipal.tipo;
        var retorno = false;
        for( var i=0; i<radios.length; i++ )
        {
            if( radios[i].checked == true){ retorno = true; }
        }
        if (!(retorno)) {
            bootbox.alert('Você deve marcar uma das opções para Tipo');
            return false;
        }

    }
</script>
<? else: ?>
<section class="alert alert-danger text-center col-md-6 col-md-offset-3">
    Você não possui permissão para acessar esta página.
</section>
<? endif; ?>
