<div >
    <div class="alert alert-warning">
        <strong>ORIENTAÇÕES PARA PREENCHIMENTO:</strong><br/><br/>
        <p class="text-justify">1 - O participante pode informar seu nível de concordância com o texto de cada artigo e apresentar, caso deseje, sugestões de alteração da redação.</p>
        <p class="text-justify">2 - <span style="color: red;">ATENÇÃO!</span> O preenchimento deste formulário poderá ser efetuado durante todo o período em que consulta pública estiver disponível. Para gravar as informações inseridas com possibilidade de alterações e complementações posteriores, clique no botão SALVAR. Para submeter sua avaliação, é necessário clicar no botão ENVIAR, e nesse caso não poderão ser feitas alterações.</p>
        <p class="text-justify">3 - Se desejar conhecer a proposta na sua íntegra, <a target="_blank" href="minutaPNF.pdf">acesse aqui o conteúdo em pdf. <span class="fa fa-file-pdf-o"> </span></a></p>
        <p class="text-justify">4 - De acordo com o disposto na Portaria n° 620, de 24 de junho de 2015, a consulta pública estará aberta até o dia 24 de julho de 2015.</p>
    </div>
</div>
<div class="well well-sm">
    <fieldset>
        <legend>Informações do participante</legend>
        <div class="row" style="padding-top: 10px;">
            <div class="col-md-3" style="padding-right: 25px;">
                <div class="form-group">
                    <label>Nome: <?php echo $particiante->parnome;?> </label>
                </div>
            </div>
            <div class="col-md-4" style="padding-right: 25px;">
                <div class="form-group">
                    <label>Data de Nascimento: <?php echo formata_data($particiante->pardatanascimento) ;?> </label>
                </div>
            </div>
            <div class="col-md-5" style="padding-right: 25px;">
                <div class="form-group">
                    <label>Sexo: <?php echo $sexo;?> </label>
                </div>
            </div>
        </div>        
        <div class="row" style="padding-top: 10px;">
            <div class="col-md-3" style="padding-right: 25px;">
                <div class="form-group">
                    <label for="estuf">UF: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted select" name="estuf" id="estuf">
                        <option value="">Selecione</option>
                        <?php foreach ($estados as $estado) { ?>
                            <?php $selected = $particiante->estuf == $estado['estuf'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $estado['estuf']; ?>"><?php echo $estado['estdescricao']; ?></option>
                        <?php } ?>
                    </select>
                </div>
            </div>
            <div class="col-md-4" style="padding-right: 25px;">
                <div class="form-group" id="div_municipio">
                    <label for="muncod">Município: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted select" id="muncod" name="muncod">
                        <option value="">Selecione</option>
                        <?php foreach ($municipios as $dados) : ?>
                            <?php $selected = $particiante->muncod == $dados['muncod'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $dados['muncod']; ?>"><?php echo ($dados['mundescricao']); ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
            <div class="col-md-5">
                <div class="form-group">
                    <label for="representacao">Tipo de Representação: </label>
                    <select disabled="disabled" disabled="disabled" class="form-control chosen-select persisted required" name="parrepresentacao" id="representacao">
                        <option value="">Selecione</option>
                        <?php foreach (Participante::$tiposRepresentacao as $id => $label): ?>
                            <?php $selected = $particiante->parrepresentacao == $id ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $id; ?>"><?php echo $label; ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
        </div>

        <div class="row" style="padding-top: 10px;">
            <div class="col-md-3" style="padding-right: 25px;">
                <div class="form-group">
                    <label for="paremail">Email: </label>
                    <input disabled="disabled" type="text" class="form-control persisted required" id="paremail" name="paremail" placeholder="Email" value="<?php echo $particiante->paremail; ?>">                
                </div>
            </div>
            <div class="col-md-4" style="padding-right: 25px;">
                <div class="form-group" id="div_municipio">
                    <label for="escid">Escolaridade: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted select" id="escid" name="escid">
                        <option value="">Selecione</option>
                        <?php foreach ($escolaridade as $dados) : ?>
                            <?php $selected = $particiante->escid == $dados['escid'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $dados['escid']; ?>"><?php echo ($dados['escdsc']); ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
        </div>
        <div class="row" style="padding-top: 10px;" >            
            <div class="col-md-7">
                <div class="form-group">
                    <label for="atuid">Principal Área de Atuação: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted required" name="atuid" id="atuid">
                        <option value="">Selecione</option>
                        <?php foreach ($atuacao as $dados) : ?>
                            <?php $selected = $particiante->atuid == $dados['atuid'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $dados['atuid']; ?>"><?php echo ($dados['atudsc']); ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>        
            <?php $selected = $particiante->atuid == 14 ? 'display:block;' : 'display:none;';?>        
            <div class="col-md-4 atuoutro" style="padding-left: 25px; <?php echo $selected;?>">
                <div class="form-group">
                    <label for="atuoutro">Outro: </label>
                    <input disabled="disabled" type="text" class="form-control persisted required" id="atuoutro" name="atuoutro" placeholder="Outro" value="<?php echo $particiante->atuoutro; ?>">
                </div>
            </div>
        </div>        
    </fieldset>
</div>
<div class="well well-sm representacao_entidade">
    <fieldset>
        <legend>Informações da Instituição</legend>
        <div class="row">
            <div class="col-md-7">
                <div class="form-group">
                    <label for="intid">Principal Área de Atuação: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted required" name="intid" id="intid">
                        <option value="">Selecione</option>
                        <?php foreach ($instituicaoTipo as $dados) : ?>
                            <?php $selected = $particiante->intid == $dados['intid'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $dados['intid']; ?>"><?php echo ($dados['intdsc']); ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>        
            <?php $selected = $particiante->intid == 13 ? 'display:block;' : 'display:none;'; ?>
            <div class="intoutro col-md-4" style="padding-left: 25px; <?php echo $selected;?>">
                <div class="form-group">
                    <label for="intoutro">Outro: </label>
                    <input disabled="disabled" type="text" class="form-control persisted required" id="intoutro" name="intoutro" placeholder="Outro" value="<?php echo $particiante->intoutro; ?>">
                </div>
            </div>
        </div>          
        <div class="row" style="padding-top: 10px;" <?php echo $selected;?>>
            <div class="col-md-3" style="padding-right: 25px;">
                <div class="form-group representacao_entidade">
                    <label for="orgao">Órgão: </label>
                    <input disabled="disabled" type="text" class="form-control representacao_entidade persisted required" id="orgao" name="parorgao" placeholder="Nome do Órgão" value="<?php echo $particiante->parorgao; ?>">
                </div>
            </div>
            <div class="col-md-4" style="padding-right: 25px;">
                <div class="form-group representacao_entidade">
                    <label for="orgao">CPF do Responsável: </label>
                    <input type="text" class="campocpf form-control representacao_entidade persisted required" id="parrepcpf" name="parrepcpf" placeholder="CPF do Responsável" value="<?php echo $particiante->parrepcpf; ?>">
                </div>
            </div>             
            <div class="col-md-5">
                <div class="form-group representacao_entidade" >
                    <label for="orgao">Nome Completo do Responsável: </label>
                    <input type="text" disabled="disabled" class="form-control representacao_entidade persisted required" id="repnome" name="repnome" placeholder="Nome Completo do Responsável" value="<?php echo $particiante->parrepnome; ?>">
                    <input disabled="disabled" type="hidden" id="parrepnome" name="parrepnome"  value="<?php echo $particiante->parrepnome; ?>"/>
                </div>
            </div>            
        </div>
        <div class="row">
            
            <div class="col-md-3" style="padding-right: 25px;">
                <div class="form-group representacao_entidade">
                    <label for="paremail">Email: </label>
                    <input disabled="disabled" type="text" class="form-control persisted required" id="parrepemail" name="parrepemail" placeholder="Email" value="<?php echo $particiante->parrepemail; ?>">                
                </div>
            </div>            
            <div class="col-md-4" style="padding-right: 25px;">
                <div class="form-group representacao_entidade" id="div_tipo_orgao">
                    <label for="cnpj">CNPJ: </label>
                    <input disabled="disabled" type="text" disabled="disabled" class="form-control" id="parcnpj" placeholder="CPNJ" value="<?php echo formatar_cnpj($particiante->parcnpj); ?>">
                </div>
            </div>
        </div>
        <div class="row">            
            <div class="col-md-15" style="padding-right: 35px;">
                <div class="form-group representacao_entidade">
                    <label for="fantasia">Nome Fantasia: </label>
                    <input disabled="disabled" type="text" disabled="disabled" class="form-control" id="fantasia" placeholder="Nome Fantasia" value="<?php echo $particiante->parreprazaosocial; ?>">
                </div>
            </div>
        </div>
        <div class="row" style="padding-top: 10px;">
            <div class="col-md-3" style="padding-right: 25px;">
                <div class="form-group">
                    <label for="parrepuf">UF: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted select" name="parrepuf" id="parrepuf">
                        <option value="">Selecione</option>
                        <?php foreach ($estados as $estado) { ?>
                            <?php $selected = $particiante->parrepuf == $estado['estuf'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $estado['estuf']; ?>"><?php echo $estado['estdescricao']; ?></option>
                        <?php } ?>
                    </select>
                </div>
            </div>
            <div class="col-md-4" style="padding-right: 25px;">
                <div class="form-group" id="div_municipio_rep">
                    <label for="parrepmuncod">Município: </label>
                    <select disabled="disabled" class="form-control chosen-select persisted select" id="parrepmuncod" name="parrepmuncod">
                        <option value="">Selecione</option>
                        <?php foreach ($municipios as $dados) : ?>
                            <?php $selected = $particiante->parrepmuncod == $dados['muncod'] ? 'selected="selected"' : null; ?>
                            <option <?php echo $selected; ?> value="<?php echo $dados['muncod']; ?>"><?php echo ($dados['mundescricao']); ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>   
        </div>
    </fieldset>
</div>	        