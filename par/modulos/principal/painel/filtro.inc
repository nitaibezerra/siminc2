<!-- Filtro-->
<div class="row clearfix filtro" style="margin-bottom: 15px; display: none">
    <div class="col-md-12 column">
        <div class="panel-group" id="panel-filtro">
            <div class="panel panel-default">
                <div class="panel-heading">
                    <a class="panel-title" data-toggle="collapse" data-parent="#panel-filtro" href="#panel-element-filtro">
                        Filtros
                    </a>
                </div>
                <div id="panel-element-filtro" class="panel-collapse collapse">
                    <div class="panel-body">
                        <div class="col-md-12 column">
                            <form role="form" action="" name="" id="frmPesquisar">
                                <div class="row clearfix">
                                    <div class="col-md-4 column">
                                        <div class="form-group">
                                            <label for="exampleInputEmail1" style="width: 100%">UF:</label>
                                            <?php
                                            $obEstadoControle = new EstadoControle();
                                            $arrEstado = $obEstadoControle->carregarUf();
                                            inputCombo("estuf[]", $arrEstado, '', '', array('multiple' => 'multiple'));
                                            ?>
                                        </div>
                                    </div>
                                    <div class="col-md-4 column">
                                        <div class="form-group">
                                            <label for="exampleInputPassword1">Município:</label>
                                            <?php
                                            $obMunicipioControle = new MunicipioControle();
                                            $arrMunicipio = $obMunicipioControle->carregarMunicipio();
                                            inputCombo("muncod[]", $arrMunicipio, '', '', array('multiple' => 'multiple'));
                                            ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row clearfix">
                                    <div class="col-md-4 column">
                                        <div class="form-group">
                                            <label for="exampleInputPassword1">Tipo de Obra:</label>
                                            <?php
                                            $obPreTipoObraControle = new PreTipoObraControle();
                                            $arrPreTipoObra = $obPreTipoObraControle->carregarPreTipoObra();
                                            inputCombo("ptoid[]", $arrPreTipoObra, '', '', array('multiple' => 'multiple'));
                                            ?>
                                        </div>
                                    </div>
                                    <div class="col-md-4 column">
                                        <div class="form-group">
                                            <label for="exampleInputPassword1" style="width: 100%">Demandado:</label>
                                            <?php
                                            $obPerfilUsuarioControle = new PerfilUsuarioControle();
                                            $arrPerfilUsuario = $obPerfilUsuarioControle->carregarPerfilUsuario(PAR_PERFIL_ENGENHEIRO_FNDE);
                                            inputCombo("usucpf[]", $arrPerfilUsuario, '', '', array('multiple' => 'multiple'));
                                            ?>
                                        </div>
                                    </div>
                                </div>
                                <button type="button" class="btn btn-primary" name="btnBuscar">Buscar</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>    
<!-- /Filtro-->