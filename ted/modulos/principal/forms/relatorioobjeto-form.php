<section class="well">
    <h4 class="text-center">Prestação de Contas do Objeto</h4>
</section>
<form class="form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      enctype="multipart/form-data"
      role="form">

    <?= $this->element->tcpid; ?>
    <?= $this->element->recid; ?>

    <div class="well">
        <h5 class="text-center">Dados da Entidade Proponente</h5>
        <br />
        <div class="form-group">
            <label class="control-label col-md-2" for="reccnpj">CNPJ:</label>
            <div class="col-md-10">
                <?= $this->element->reccnpj; ?>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="recnome">Nome da Entidade:</label>
            <div class="col-md-10">
                <?= $this->element->recnome;?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recendereco">Endereço:</label>
            <div class="col-md-10">
                <?= $this->element->recendereco; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="estuf">UF:</label>
            <div class="col-md-10">
                <?= $this->element->estuf; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="muncod">Município:</label>
            <div class="col-md-10" id="div-muncod">
                <?= $this->element->muncod; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="reccep">CEP:</label>
            <div class="col-md-10">
                <?= $this->element->reccep; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="rectelefoneddd">DDD:</label>
            <div class="col-md-2">
                <?= $this->element->rectelefoneddd; ?>
            </div>
            <label class="control-label col-md-2" for="rectelefone">Telefone:</label>
            <div class="col-md-6">
                <?= $this->element->rectelefone; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="uocod">Código da UO:</label>
            <div class="col-md-10">
                <?= $this->element->uocod; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="ugcod">Código da UG:</label>
            <div class="col-md-10">
                <?= $this->element->ugcod; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="gestaocod">Código da Gestão:</label>
            <div class="col-md-10">
                <?= $this->element->gestaocod; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recnomeresponsavel">Nome do Responsável:</label>
            <div class="col-md-10">
                <?= $this->element->recnomeresponsavel; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="reccpfresponsavel">CPF do Responsável:</label>
            <div class="col-md-10">
                <?= $this->element->reccpfresponsavel; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recsiaperesponsavel">SIAPE do Responsável:</label>
            <div class="col-md-10">
                <?= $this->element->recsiaperesponsavel; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recrgresponsavel">Identidade do Responsável:</label>
            <div class="col-md-10">
                <?= $this->element->recrgresponsavel; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recdtemissaorgresposavel">Data de Emissão:</label>
            <div class="col-md-10">
                <?= $this->element->recdtemissaorgresposavel; ?>
                <p class="help-block">Formato para preenchimento da data. Exemplo 02/10/2015.</p>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recexpedidorrgresposavel">Orgão Expedidor:</label>
            <div class="col-md-1">
                <?= $this->element->recexpedidorrgresposavel; ?>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="reccargo">Cargo:</label>
            <div class="col-md-10">
                <?= $this->element->reccargo; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recemailresposavel">E-mail do Responsável:</label>
            <div class="col-md-10">
                <?= $this->element->recemailresposavel; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recnumportaria">Nº da Portaria ou Decreto de Nomeação:</label>
            <div class="col-md-10">
                <?= $this->element->recnumportaria; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recdtpublicacao">Data de Publicação:</label>
            <div class="col-md-10">
                <?= $this->element->recdtpublicacao; ?>
                <p class="help-block">Formato para preenchimento da data. Exemplo: 02/10/2015.</p>
            </div>
        </div>
    </div>

    <div class="well">
        <h5 class="text-center">Dados do Objeto da Descentralização do Crédito</h5>
        <br />
        <div class="form-group">
            <label class="control-label col-md-2" for="recnumnotacredito">Nota de Crédito:</label>
            <div class="col-md-7">
                <?= $this->element->recnumnotacredito; ?>
                <?php $model = new Ted_Model_RelatorioCumprimento(); ?>
                <div id="div-nc">
                    <?php $model->mostraNc(); ?>
                </div>
            </div>
            <div class="col-md-1">
                <button class="btn btn-primary btn-sm add-nc">Adicionar</button>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="recexecucaoobjeto">Execução do Objeto:</label>
            <div class="col-md-10">
                <?= $this->element->recexecucaoobjeto; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recatividadesprevistas">Atividades Previstas:</label>
            <div class="col-md-10">
                <?= $this->element->recatividadesprevistas; ?>
                <div id="counter-recatividadesprevistas" class=""></div>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="recmetaprevista">Meta Prevista:</label>
            <div class="col-md-10">
                <?= $this->element->recmetaprevista; ?>
                <div id="counter-recmetaprevista" class=""></div>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recatividadesexecutadas">Atividades Executadas:</label>
            <div class="col-md-10">
                <?= $this->element->recatividadesexecutadas; ?>
                <div id="counter-recatividadesexecutadas" class=""></div>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="recmetaexecutada">Meta Executada:</label>
            <div class="col-md-10">
                <?= $this->element->recmetaexecutada; ?>
                <div id="counter-recmetaexecutada" class=""></div>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recdificuldades">Dificuldades Encontradas na Execução da Descentralização:</label>
            <div class="col-md-10">
                <?= $this->element->recdificuldades; ?>
                <div id="counter-recdificuldades" class=""></div>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="recmetasadotadas">Medidas Adotadas para Sanar as Dificuldades de Modo a Assegurar o Cumprimrnto do Objeto:</label>
            <div class="col-md-10">
                <?= $this->element->recmetasadotadas; ?>
                <div id="counter-recmetasadotadas" class=""></div>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="reccomentarios">Comentários Adicionais:</label>
            <div class="col-md-10">
                <?= $this->element->reccomentarios; ?>
                <div id="counter-reccomentarios" class=""></div>
            </div>
        </div>
    </div>

    <div class="well">
        <h5 class="text-center">Detalhamento do Crédito Orçamentário Recebido</h5>
        <br />

        <div class="form-group">
            <label class="control-label col-md-2" for="recnumnotacredito_dev">NC de Devolução:</label>
            <div class="col-md-7">
                <?= $this->element->recnumnotacredito_dev; ?>
                <div id="div-nc-devolucao">
                    <? $model->mostraNcDevolucao(); ?>
                </div>
            </div>
            <div class="col-md-1">
                <button class="btn btn-primary btn-sm add-nc-dev">Adicionar</button>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recvlrrecebido">Valor Recebido (R$ 1,00):</label>
            <div class="col-md-10">
                <?= $this->element->recvlrrecebido; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recvlrutilizado">Valor Utilizado (R$ 1,00):</label>
            <div class="col-md-10">
                <?= $this->element->recvlrutilizado; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="recvlrdevolvido">Valor Devolvido (R$ 1,00):</label>
            <div class="col-md-10">
                <?= $this->element->recvlrdevolvido; ?>
            </div>
        </div>

        <div class="form-group">
            <label class="control-label col-md-2" for="arquivo">Anexar arquivo:</label>
            <div class="col-md-10">
                <input type="file" class="btn start" name="arquivo" id="arquivo">
            </div>
        </div>
    </div>

    <div class="form-group">
        <div class="col-md-offset-2">
            <input type="reset" class="btn btn-warning" name="cancel" id="cancel" value="Cancelar">
            <input type="submit" class="btn btn-primary" name="enviar" id="enviar" value="Gravar">
        </div>
    </div>

</form>