<?php

//declarando objetos
$comboVersion = new Ted_Form_Historico();
$historico = new Ted_Model_Historico();

//Alimenta o combo de versões do termo se houver
if (count($historico->getComboHistorico())) {
    $comboVersion->getElement('version')->addMultiOptions($historico->getComboHistorico());
} else {
    $comboVersion->getElement('version')->addMultiOption('', '- Não existe histórico -');
}

//Receive requests
if (isset($_GET['versao']) && isset($_GET['ted'])) {
    $comboVersion->getElement('version')->setValue($_GET['versao']);
    $historico->setVersion($_GET['versao']);
}

?>
<style type="text/css">
ul.list-file {
    list-style: none;
    padding: 0;
    margin: 0;
}
.list-file li {
    display: block;
    float: left;
    margin: 40px;
    height: 40px;
    padding: 0;
}
</style>

<script type="text/javascript">
$(function() {
    $("#version").on("change", function() {
        if ($(this).val()) {
            var urlbase = "ted.php?modulo=principal/termoexecucaodescentralizada/historico&acao=A&ted=";
            location.href = urlbase+"<?= Ted_Utils_Model::capturaTcpid();?>&versao="+$(this).val();
        }
    });

    $("#version_chosen").attr("style", "width: 100%;");
});
</script>
<br />
<section>
    <?php echo $comboVersion->showForm(); ?>
</section>

<?php if ($historico->getVersion()) : ?>
    <section>
        <?php $hst = $historico->get(); ?>
    </section>

    <section>
        <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
            <tr>
                <th colspan="4" class="text-center well">Proponente</th>
            </tr>
            <tr>
                <th>Unidade Gestora</th>
                <th>CNPJ</th>
                <th>Endereço</th>
                <th>Bairro</th>
            </tr>
            <tr>
                <td><?= $hst['proponente']['ungcod'] .' - '.$hst['proponente']['razao']; ?></td>
                <td><?= formatar_cnpj($hst['proponente']['ungcnpj']); ?></td>
                <td><?= $hst['proponente']['ungendereco']; ?></td>
                <td><?= $hst['proponente']['ungbairro']; ?></td>
            </tr>
            <tr>
                <th>UF - Município</th>
                <th>CEP</th>
                <th>Telefone</th>
                <th>E-mail</th>
            </tr>
            <tr>
                <td><?= $hst['proponente']['estuf'] .' - '.$hst['proponente']['municipio']; ?></td>
                <td><?= $hst['proponente']['ungcep']; ?></td>
                <td><?= $hst['proponente']['ungfone']; ?></td>
                <td><?= $hst['proponente']['ungemail']; ?></td>
            </tr>
            <?php //var_dump($hst['proponente']['coordenacao']); ?>
            <?php if ($hst['proponente']['coordenacao']) : ?>
                <tr>
                    <th colspan="2">Nome da Coordenação</th>
                    <th colspan="2">Telefone</th>
                </tr>
                <tr>
                    <td><?= $hst['proponente']['coordenacao']['nomecoordenacao']; ?></td>
                    <td>(<?= $hst['proponente']['coordenacao']['dddcoordenacao'].') '.$hst['proponente']['coordenacao']['telefonecoordenacao']; ?></td>
                </tr>
            <?php endif; ?>
            <tr>
                <th colspan="4" class="text-center well">Representante legal do Proponente</th>
            </tr>
            <tr>
                <th colspan="2">Nome</th>
                <th>CPF</th>
                <th>E-mail</th>
            </tr>
            <tr>
                <td colspan="2"><?= utf8_decode($hst['proponente']['rlp']['nome']); ?></td>
                <td><?= formatar_cpf($hst['proponente']['rlp']['cpf']); ?></td>
                <td><?= $hst['proponente']['rlp']['email']; ?></td>
            </tr>
            <?php if ($hst['proponente']['rlps']): ?>
                <tr>
                    <th colspan="4" class="text-center well">Representante legal do Proponente Substituto</th>
                </tr>
                <tr>
                    <th colspan="2">Nome</th>
                    <th>CPF</th>
                    <th>E-mail</th>
                </tr>
                <tr>
                    <td colspan="2"><?= $hst['proponente']['rlps']['nome']; ?></td>
                    <td><?= formatar_cpf($hst['proponente']['rlps']['cpf']); ?></td>
                    <td><?= $hst['proponente']['rlps']['email']; ?></td>
                </tr>
            <?php endif ?>
        </table>
    </section>

    <section>
        <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
            <tr>
                <th colspan="4" class="text-center well">Concedente</th>
            </tr>
            <tr>
                <th>Unidade Gestora</th>
                <th>CNPJ</th>
                <th>Endereço</th>
                <th>Bairro</th>
            </tr>
            <tr>
                <td><?= $hst['concedente']['ungcod'] .' - '.$hst['concedente']['razao']; ?></td>
                <td><?= formatar_cnpj($hst['concedente']['ungcnpj']); ?></td>
                <td><?= $hst['concedente']['ungendereco']; ?></td>
                <td><?= $hst['concedente']['ungbairro']; ?></td>
            </tr>
            <tr>
                <th>UF - Município</th>
                <th>CEP</th>
                <th>Telefone</th>
                <th>E-mail</th>
            </tr>
            <tr>
                <td><?= $hst['concedente']['estuf'] .' - '.$hst['concedente']['municipio']; ?></td>
                <td><?= $hst['concedente']['ungcep']; ?></td>
                <td><?= $hst['concedente']['ungfone']; ?></td>
                <td><?= $hst['concedente']['ungemail']; ?></td>
            </tr>
            <?php //var_dump($hst['concedente']['coordenacao']); ?>
            <?php if ($hst['concedente']['coordenacao']) : ?>
                <tr>
                    <th colspan="2">Nome da Coordenação</th>
                    <th colspan="2">Telefone</th>
                </tr>
                <tr>
                    <td><?= $hst['concedente']['coordenacao']['nomecoordenacao']; ?></td>
                    <td>(<?= $hst['concedente']['coordenacao']['dddcoordenacao'].') '.$hst['concedente']['coordenacao']['telefonecoordenacao']; ?></td>
                </tr>
            <?php endif; ?>
            <tr>
                <th colspan="4" class="text-center well">Representante legal do Concedente</th>
            </tr>
            <tr>
                <th colspan="2">Nome</th>
                <th>CPF</th>
                <th>E-mail</th>
            </tr>
            <tr>
                <td colspan="2"><?= utf8_decode($hst['concedente']['rlp']['nome']); ?></td>
                <td><?= formatar_cpf($hst['concedente']['rlp']['cpf']); ?></td>
                <td><?= $hst['concedente']['rlp']['email']; ?></td>
            </tr>
            <?php if ($hst['concedente']['rlps']): ?>
                <tr>
                    <th colspan="4" class="text-center well">Representante legal do Concedente Substituto</th>
                </tr>
                <tr>
                    <th colspan="2">Nome</th>
                    <th>CPF</th>
                    <th>E-mail</th>
                </tr>
                <tr>
                    <td colspan="2"><?= $hst['concedente']['rlps']['nome']; ?></td>
                    <td><?= formatar_cpf($hst['concedente']['rlps']['cpf']); ?></td>
                    <td><?= $hst['concedente']['rlps']['email']; ?></td>
                </tr>
            <?php endif ?>
        </table>
    </section>

    <section>
        <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
            <tr>
                <th colspan="4" class="text-center well">Justificativa</th>
            </tr>
            <tr>
                <th>Identificação</th>
                <th>Objetivo</th>
                <th>Justificativa</th>
                <th>Tipo Emenda</th>
            </tr>
            <tr>
                <td><?= $hst['justificativa']['identificacao']; ?></td>
                <td><?= $hst['justificativa']['objetivo']; ?></td>
                <td><?= $hst['justificativa']['justificativa']; ?></td>
                <td><?= ($hst['justificativa']['tipoemenda'] == 'N') ? 'Não' : 'Sim'; ?></td>
            </tr>
        </table>
    </section>

    <section>
        <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
            <tr>
                <th colspan="4" class="text-center well">Previsão Orçamentária</th>
            </tr>
            <tr>
                <th>Ano</th>
                <th>Ação</th>
                <th>Programa <br>de Trabalho</th>
                <th>Plano <br>Interno</th>
                <th>Descrição <br>da Ação <br>Constante <br>da LOA</th>
                <th>Natureza <br>de Despesa</th>
                <th>Valor (em R$)</th>
                <th>Mês <br>de liberação</th>
                <th>Prazo para o <br>cumprimento <br>do objeto</th>
            </tr>
            <?php if($hst['previsao']): ?>
                <?php foreach($hst['previsao'] as $previsao): ?>
                    <tr>
                        <td class="text-center"><?= $previsao['proanoreferencia']; ?></td>
                        <td class="text-center"><?= $previsao['acao']; ?></td>
                        <td><?= $previsao['programatrabalho']; ?></td>
                        <td><?= $previsao['planointerno']; ?></td>
                        <td><?= $previsao['acadsc']; ?></td>
                        <td><?= $previsao['naturezadespesa']; ?></td>
                        <td class="text-left"><?= trim($previsao['provalor']); ?></td>
                        <td class="text-center"><?= $previsao['crdmesliberacao']; ?></td>
                        <td class="text-center"><?= $previsao['crdmesexecucao']; ?></td>
                    </tr>
                <?php endforeach; ?>
            <?php endif; ?>
        </table>
    </section>

    <section>
        <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
            <tr>
                <th colspan="4" class="text-center well">Parecer Técnico (Diretoria)</th>
            </tr>
            <tr>
                <th class="text-left">Considerações sobre a entidade proponente</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considentproponente']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Considerações sobre a proposta</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considproposta']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Considerações sobre o objeto</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considobjeto']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Considerações sobre o objetivo</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considobjetivo']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Considerações sobre a justificativa</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considjustificativa']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Considerações sobre os valores</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considvalores']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Outras considerações cabíveis</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['considcabiveis']; ?></td>
            </tr>
            <tr>
                <th class="text-left">Parecer Técnico elaborado por</th>
            </tr>
            <tr>
                <td><?= $hst['parecer']['usucpfparecer']; ?></td>
            </tr>
        </table>
    </section>

    <section>
        <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
            <tr>
                <th colspan="4" class="text-center well">Anexos</th>
            </tr>
            <tr>
                <td colspan="2">
                    <?php if ($hst['anexos']) : ?>
                        <ul class="list-file">
                            <?php foreach ($hst['anexos'] as $arq) : ?>
                                <li id="<?= $arq['arqid'] ?>">
                                    <?= $arq['arpdsc']; ?><br>
                                    <em><?= $arq['arpdtinclusao'] ?></em>
                                </li>
                            <?php endforeach; ?>
                        </ul>
                    <?php endif; ?>
                </td>
            </tr>
        </table>
    </section>
<?php endif; ?>

<?php //$t = $historico->get(); ?>
<?php //ver($t['anexos']); ?>