<form class="form-horizontal"
      name="solicitarNC"
      id="solicitarNC"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

    <?= $this->element->tcpid; ?>
    <?= $this->element->especie; ?>
    <?= $this->element->sistema; ?>
    <?= $this->element->evento_contabil; ?>
    
	<input type="hidden" name="funcao" id="funcao" value="fndesolicitanc" />

    <?php if (!$listaPO) : ?>
    	<div id="divMsg" class="divMsg" style="display: none;">
	        <script type="text/javascript"> //desabilitaEnvioNc(); </script>
	        <section class="alert alert-success text-center col-md-12">
	            <span class="glyphicon glyphicon-ok"></span>
	            A Solicitação da Nota de Crédito foi enviada para o FNDE.
	        </section>    
	        <br style="clear:both;"/>
        </div>
    <?php endif ?>

    <div class="form-group">
        <label class="control-label col-md-3" for="sigefusername">Usuário do SIGEF:</label>
        <div class="col-md-9">
            <?= $this->element->sigefusername; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="sigefpassword">Senha do SIGEF:</label>
        <div class="col-md-9">
            <?= $this->element->sigefpassword; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="tcpnumtransfsiafi">Número de Transferência:</label>
        <div class="col-md-9">
            <?= $this->element->tcpnumtransfsiafi; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="tcpnumprocessofnde">Processo:</label>
        <div class="col-md-9">
            <?= $this->element->tcpnumprocessofnde; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="unicod">Cód. Unidade Orçamentária:</label>
        <div class="col-md-9">
            <?= $this->element->unicod; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="ungcodemitente">Cód. Unidade Gestora Emitente:</label>
        <div class="col-md-9">
            <?= $this->element->ungcodemitente; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="gescodemitente">Cód. Centro de Gestão Emitente:</label>
        <div class="col-md-9">
            <?= $this->element->gescodemitente; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="tcpprogramafnde">Programa:</label>
        <div class="col-md-9">
            <?= $this->element->tcpprogramafnde; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="tcpobsfnde">Observação:</label>
        <div class="col-md-9" id="div-observacao">
            <?= $this->element->tcpobsfnde; ?>
        </div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-3" for="tcpobscomplemento">Complemento da Observação:</label>
        <div class="col-md-9">
            <?= $this->element->tcpobscomplemento; ?>
        </div>
    </div>
    
    <h3>Previsões Orçamentárias</h3>
    <?php
	$prevOrcamentaria = new Ted_Model_PrevisaoOrcamentaria();
	
	/* Captura todas as previsões orçamentárias que não foram enviadas para pagamento do atual termo. */
	$listaPO = $prevOrcamentaria->listaPrevisaoOrcamentariaEnviarNC($_GET['ted']);

    if ($listaPO) {
        foreach ($listaPO as $po) {
            $rsPrevisao = $prevOrcamentaria->PegaPrevisaoOrcamentariaEnviarNC($po['proid']);

            //Declaração de variáveis a serem utilizadas no formulário abaixo.
            $prgid = $rsPrevisao['prgidfnde'];
            $esfid = $rsPrevisao['esfid'] ? $rsPrevisao['esfid'] : 1;
            $espid = $rsPrevisao['espid'] ? $rsPrevisao['espid'] : 3;
            $prgfonterecurso = $rsPrevisao['prgfonterecurso'];
            $titleComboCelula = "Código Programa FNDE - Plano Interno - Centro Gestão - Tipo Documento - Observação - Evento Contábil";
    ?>
            <div class="bs-callout bs-callout-info" id="bs<?=$rsPrevisao['proid']?>">
                <table class="table table-bordered table-striped table-responsive table-condensed">
                    <tr>
                        <th>Ano</th>
                        <th>Ação</th>
                        <th>Programa de Trabalho</th>
                        <th>Plano interno</th>
                        <th>Descrição da Ação constante da LOA</th>
                        <th>Natureza da despesa</th>
                        <th>Valor (R$)</th>
                        <th>Mês da liberação</th>
                        <th>Prazo para o cumprimento do objeto (meses)</th>
                    </tr>
                    <tr>
                        <td><?=$rsPrevisao['proanoreferencia'] ?></td>
                        <td><?=$rsPrevisao['acacod']?></td>
                        <td><code><?=$rsPrevisao['ptrid_descricao'] ?></code></td>
                        <td><?=$rsPrevisao['pliid_descricao'] ?></td>
                        <td><?=$rsPrevisao['acatitulo'] ?></td>
                        <td><?=$rsPrevisao['ndp_descricao'] ?></td>
                        <td><b><?=$rsPrevisao['provalor'] ?></b></td>
                        <td><?=$rsPrevisao['crdmesliberacao'] ?></td>
                        <td><?=$rsPrevisao['crdmesexecucao'] ?></td>
                    </tr>
                </table>

                <div class="row well well-sm" style="margin-bottom: 0;">
                    <div class="col-md-1">
                        <input  name="chekCel[]" value="<?=$po['proid']?>" type="checkbox" title="Selecione caso queira enviar" checked/>
                    </div>

                    <div class="form-group col-md-3">
                        <label class="control-label">Célula Orçamentaria</label>
                        <div class="">
                        <?php
                        $_programa = isset($_REQUEST['prgid'][$po['proid']]) ? $_REQUEST['prgid'][$po['proid']] : $prgid;            
                        $celulaOrcamentaria = $prevOrcamentaria->pegaCelulaOrcamentariaEnviarNC($po['plicod']);

                        if ($celulaOrcamentaria){
                            inputCombo("prgid[{$po['proid']}]", $celulaOrcamentaria, $_programa, "prgid[{$po['proid']}]", array('title' => $titleComboCelula));
                        } else {
                            echo '<p class=\"control-static-form\">Nada encontrado</p>';
                        }
                        ?>
                        </div>
                    </div>
                    <div class="form-group col-md-3">
                        <label class="control-label">Espécie</label>
                        <div class="">
                        <?php
                        $especie = $prevOrcamentaria->listaEspecieNC();
                        inputCombo("espid[{$po['proid']}]", $especie, $espid, "espid[{$po['proid']}]", array());
                        ?>
                        </div>
                    </div>
                    <div class="form-group col-md-3">
                        <label class="control-label">Esfera</label>
                        <div class="">
                        <?php
                        inputCombo("esfid[{$po['proid']}]", $prevOrcamentaria->listaEsferaNC() , $esfid, "esfid[{$po['proid']}]", array());
                        ?>
                        </div>
                    </div>
                    <div class="form-group col-md-3">
                        <label class="control-label">Cód. Fonte do Recurso</label>
                        <div class="">
                        <?php
                        $valor = isset($_REQUEST['prgfonterecurso'][$po['proid']]) ? $_REQUEST['prgfonterecurso'][$po['proid']] : null;
                        inputCombo("prgfonterecurso[{$po['proid']}]", $prevOrcamentaria->listaFonteRecursoNC(), $valor, "prgfonterecurso[{$po['proid']}]", array());
                        ?>
                        </div>
                    </div>
                </div>
            </div>
        <?php
        }
    }
    ?>
    <button type="submit" class="btn btn-primary" id="enviarWS">Solicitar Nota de Crédito</button>
</form>
