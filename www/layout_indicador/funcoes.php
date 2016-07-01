<?php

function montarUfs($regioes = array())
{
    global $db;

    $where = is_array($regioes) && count($regioes) ? " where regcod in ('" . implode("', '", $regioes) . "') " : '';
    ?>

    <table>
        <tr>
            <?php
            $sql = "SELECT	estuf, estdescricao
                    FROM territorios.estado
                    $where
                    ORDER BY estuf ";
            $arrDados = $db->carregar($sql);

            foreach ($arrDados as $dados) {
                $active = is_array($_POST['ufs']) && in_array($dados['estuf'], $_POST['ufs']);
                ?>
                <td>
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default <?php echo $active ? 'active' : ''; ?>">
                            <input type="checkbox" class="checkbox-uf" name="estuf[]" autocomplete="off" value="<?php echo $dados['estuf'];?>" <?php echo $active ? 'checked="checked"' : ''; ?>>
                            <img width="15px" src="/imagens/bandeiras/mini/<?php echo $dados['estuf']; ?>.png"><br>
                            <div style="font-size: 10px">
                                <?php echo $dados['estuf'];?>
                            </div>
                        </label>
                    </div>
                </td>
            <?php } ?>
        </tr>
    </table>

<?php }

function montarMunicipio($requisicao = array())
{
    global $db;
    $join = array();

    if((isset($requisicao['estuf']) && is_array($requisicao['estuf'])) || (isset($requisicao['gtmid']) && is_array($requisicao['gtmid'])) || (isset($requisicao['tpmid']) && is_array($requisicao['tpmid']))){
        $where = ' where true ';

        if (isset($requisicao['estuf']) && is_array($requisicao['estuf'])) {
            $where .= " and m.estuf in ('" . implode ("', '", $requisicao['estuf']) . "') ";
        }
        if (isset($requisicao['tpmid']) && is_array($requisicao['tpmid'])) {
            $where .= " and mtm.tpmid in ('" . implode ("', '", $requisicao['tpmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod ';
        } elseif (isset($requisicao['gtmid']) && is_array($requisicao['gtmid'])) {
            $where .= " and tm.gtmid in ('" . implode ("', '", $requisicao['gtmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod ';
            $join[] = ' inner join territorios.tipomunicipio tm on tm.tpmid = mtm.tpmid ';
        }
    } else {
        $where = ' where false ';
    }


    $sql = "select distinct m.muncod, m.estuf, m.mundescricao
            from territorios.municipio m
            " . implode(' ', $join).  "
            $where
            order by m.estuf, m.mundescricao";

    $municipios = $db->carregar($sql);
    $municipios = $municipios ? $municipios : array();
    ?>

    <select name="muncod[]" id="muncod" class="form-control chosen-select" multiple data-placeholder="Selecione">
        <?php foreach ($municipios as $dado) { ?>
            <option <?php echo is_array($requisicao['muncod']) && in_array($dado['muncod'], $requisicao['muncod']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['muncod']; ?>"><?php echo $dado['estuf'] . ' - ' . $dado['mundescricao']; ?></option>
        <?php } ?>
    </select>
<?php }

function montarIndicadores($requisicao = array())
{
    global $db;
    $where = "where indstatus = 'A'";
    $join = array();

    if(is_array($_POST['temas'])){
        $where .= ' and it.temid in (' . implode(', ', $requisicao['temas']) . ') ';
        $join[] = ' inner join painel.indicadortemamec it on it.indid = i.indid ';
    }
    if(is_array($_POST['etapas'])){
        $where .= ' and ie.etpid in (' . implode(', ', $requisicao['etapas']) . ') ';
        $join[] = ' inner join painel.indicadoretapaeducacao ie on ie.indid = i.indid ';
    }

    $sql = "select distinct i.indid, i.indid || ' - ' || i.indnome as indicador, i.indnome
            from painel.indicador i
                " . implode(' ', $join).  "
            {$where}
            order by i.indnome
            ";

    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();
    ?>

    <select name="indicadores[]" id="campo_indicadores" class="chosen form-control" multiple data-placeholder="Selecione">
        <?php foreach ($dados as $dado) { ?>
            <option value="<?php echo $dado['indid']; ?>" <?php echo is_array($requisicao['indicadores']) && in_array($dado['indid'], $requisicao['indicadores']) ? 'selected="selected"' : ''; ?>><?php echo $dado['indicador']; ?></option>
        <?php } ?>
    </select>
<?php }

function montarDetalhesEscola($dados)
{
    ?>
    <h1 style="color:#414145"><?php echo $dados['no_entidade']; ?></h1>
    <h2 style="color:#414145"><?php echo $dados['estuf'] . " - " . $dados['mundescricao'] ; ?></h2>

    <fieldset class="info-fieldset">
        <legend>Informações Gerais</legend>
        <label class="info-legenda" for="">Qtd. Alunos: <span class="badge alert-success"><?php echo $dados['num_alunos_existentes']; ?></span></label><br/>
        <label class="info-legenda" for="">Qtd. Funcionários: <span class="badge alert-success"><?php echo $dados['num_funcionarios']; ?></span></label><br/>
        <label class="info-legenda" for="">Qtd. Salas Utilizadas: <span class="badge alert-success"><?php echo $dados['num_salas_utilizadas']; ?></span></label><br/>
    </fieldset>

    <fieldset class="info-fieldset">
        <legend>Informática</legend>
        <label class="info-legenda" for="">Possui Internet: <?php echo $dados['id_internet'] ? '<span class="badge alert-success">Sim</span>' : '<span class="badge alert-danger">Não</span>'; ?></label><br/>
        <label class="info-legenda" for="">Possui Banda Larga: <?php echo $dados['id_banda_larga'] ? '<span class="badge alert-success">Sim</span>' : '<span class="badge alert-danger">Não</span>'; ?></label><br/>
        <label class="info-legenda" for="">Qtd. Computadores Administrativos: <span class="badge alert-success"><?php echo $dados['num_comp_administrativos']; ?></span></label><br/>
        <label class="info-legenda" for="">Qtd. Computadores para Alunos: <span class="badge alert-success"><?php echo $dados['num_comp_alunos']; ?></span></label><br/>
    </fieldset>

    <fieldset class="info-fieldset">
        <legend>Modalidades de Ensino</legend>

        <table class="table table-bordered table-striped table-hover table-condensed">
            <thead>
                <tr>
                    <th>Regular</th>
                    <th>Especial</th>
                    <th>EJA</th>
                </tr>
                <tr>
                    <th>Infantil</th>
                    <th>Fundamental</th>
                    <th>Médio</th>

                    <th>Infantil</th>
                    <th>Fundamental</th>
                    <th>Médio</th>
                    <th>EJA Fundamental</th>
                    <th>EJA Médio</th>
                </tr>
            </thead>
        </table>

    </fieldset>
	<?php
}

function detalharIndicador($request)
{
    $indid = array_pop($request['indicadores']);
    $with = $where = $whereWith = $order = '';

    if(isset($request['ufs']) && is_array($request['ufs'])){
        $where .= " and d.estuf in ('" . implode("', '", $request['ufs']) . "') ";
        $order = 'ORDER BY esc.escuf, esc.escmunicipio, esc.escdsc';
    }

    if(is_array($request['indicadores']) && count($request['indicadores'])){
        foreach ($request['indicadores'] as $count => $indicador) {
            $with[] = "
            tmp{$count} as  ( SELECT esc.esccodinep FROM painel.v_detalheindicadorsh d INNER JOIN painel.escola esc ON esc.esccodinep = d.dshcod::character(20) WHERE  d.indid=$indicador  $where ) ";

            $whereWith .= "
            and exists ( select 1 from tmp{$count} t where t.esccodinep = esc.esccodinep )";
        }

        $with = 'with ' . implode (', ', $with) ;
    }
    global $db;
    $sql = "
            {$with}

            SELECT distinct esc.*
            FROM painel.v_detalheindicadorsh d
                INNER JOIN painel.escola esc ON esc.esccodinep = d.dshcod::character(20)
            WHERE  d.indid='{$indid}'
            $where
            $whereWith
            $order
            ";

    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();

    $dadosAgrupados = array();
    foreach($dados as $dado){
        if($dado['escuf']){
            $dadosAgrupados['uf'][$dado['escuf']] = $dado['escuf'];
        }
        if($dado['escmunicipio']){
            $dadosAgrupados['municipio'][$dado['escmunicipio']] = $dado['escmunicipio'];
        }
    }

    ?>
    <h1 style="color:#414145">
        <?php echo $request['indnome']; ?>
    </h1>

    <style>
        .detalheIndicadorDados{
            color: red !important;
            font-size: 20px !important;
        }
		.modal-detalhe:hover {
			cursor: pointer;
		}
    </style>
    
    <p style="padding: 20px 0px 20px;">
        <span class="detalheIndicadorDados">
        	<span class="badge" style="background-color: #d9534f !important; color: #fff !important;"><?php echo simec_number_format(count($dados), 0); ?></span>
        </span> Escolas em
        <span class="detalheIndicadorDados">
       		<span class="badge" style="background-color: #d9534f !important; color: #fff !important;"><?php echo simec_number_format(count($dadosAgrupados['municipio']), 0); ?></span>
       	</span> Municípios de
        <span class="detalheIndicadorDados">
        	<span class="badge" style="background-color: #d9534f !important; color: #fff !important;"><?php echo simec_number_format(count($dadosAgrupados['uf']), 0); ?></span>
        </span> UFs
    </p>

    <div class="ibox float-e-margins well">
    	<div class="ibox-title">
    		<h5 class="text-center">Resultado da pesquisa</h5>
    	</div>
    	<div class="ibox-content">
	        <table class="table table-hover data-table">
	        	<thead>
	            <tr>
	                <th>UF</th>
	                <th>Município</th>
	                <th>Escola</th>
	            </tr>
	            </thead>
	            <?php foreach ($dados as $count => $dado) { ?>
	                <tr class="modal-detalhe" inep="<?php echo $dado['esccodinep']; ?>">
	                    <td><?php echo $dado['escuf']; ?></td>
	                    <td><?php echo $dado['escmunicipio']; ?></td>
	                    <td><?php echo $dado['escdsc']; ?></td>
	                </tr>
	                <?php
	                if($count == 10000){ break; }
	            } ?>
	        </table>
		</div>
    </div>

    <script type="text/javascript">
        $(function(){
        	$(".modal-detalhe").off("click").on("click", function () {
                var inep = $(this).attr('inep');
                $('#modal-conteudo').html('').load('index.php?carregarDetalheModal=1&regid=2&inep=' + inep, function(){
                    $('#modal-detalhe').modal();
                });
            });
            
        	$('.data-table').DataTable({
        		'aoColumnDefs' : [{
        			'bSortable' : false,
        			'aTargets' : [ 'unsorted' ]
        		}],
        		'oLanguage' : {
        			'sProcessing' : "Processando...",
        			'sLengthMenu' : "Mostrar _MENU_ registros",
        			'sZeroRecords' : "N&atilde;o foram encontrados resultados",
        			'sInfo' : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
        			'sInfoEmpty' : "Mostrando de 0 at&eacute; 0 de 0 registros",
        			'sInfoFiltered' : "(filtrado de _MAX_ registros no total)",
        			'sInfoPostFix' : ".",
        			'sSearch' : "Pesquisar :&nbsp;&nbsp;",
        			'sUrl' : "",
        			'oPaginate' : {
        				'sFirst' : "Primeiro",
        				'sPrevious' : "Anterior",
        				'sNext' : "Seguinte",
        				'sLast' : "&Uacute;ltimo"
        			}
        		}
        	});
        });
    </script>

<?php }

function detalharIndicadorGraficos($request)
{
    global $db;
    $grafico = new Grafico(null, false);

    $periodoInicio = 150;
    $periodoFim = 1224;

    // Gráfico Linha Comparativo
    if(isset($request['indicadores']) && is_array($request['indicadores'])){

        $whereLinha = array();

        $where = $whereWith = $with = $sqlUnion = $sqlWith = '';
        $join = array();

        if (isset($request['regcod']) && is_array($request['regcod'])) {
            $where .= " and d.regcod in ('" . implode ("', '", $request['regcod']) . "') ";
        }
        if (isset($request['estuf']) && is_array($request['estuf'])) {
            $where .= " and d.estuf in ('" . implode ("', '", $request['estuf']) . "') ";
        }
        if (isset($request['muncod']) && is_array($request['muncod'])) {
            $where .= " and d.muncod in ('" . implode ("', '", $request['muncod']) . "') ";
        }
        if (isset($request['tpmid']) && is_array($request['tpmid'])) {
            $where .= " and mtm.tpmid in ('" . implode ("', '", $request['tpmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = d.muncod ';
        } elseif (isset($request['gtmid']) && is_array($request['gtmid'])) {
            $where .= " and tm.gtmid in ('" . implode ("', '", $request['gtmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = d.muncod ';
            $join[] = ' inner join territorios.tipomunicipio tm on tm.tpmid = mtm.tpmid ';
        }

        $filtrosEscola = recuperarFiltrosEscola();
        foreach ($filtrosEscola as $campo) {
            if (isset($request[$campo]) && $request[$campo] != 'T') {
                $where .= " and esc.{$campo} = '{$request[$campo]}' ";
                $join[] = ' inner join educacenso_2014.tab_dado_escola esc on esc.fk_cod_entidade::int = d.esccodinep::int ';
            }
        }

        $join = array_unique($join);

        foreach($request['indicadores'] as $key => $indid){

            $sql = "select ind.indid, substring(ind.indnome, 0, 50) || '...' as indnome, ind.perid, ume.umedesc, ind.indcumulativo, ind.indcumulativovalor, ind.unmid, ind.regid,
                ind.indqtdevalor, ind.indshformula, ind.formulash
            from painel.indicador ind
                inner join painel.periodicidade per ON per.perid = ind.perid
                inner join painel.unidademeta ume ON ind.umeid = ume.umeid
                inner join painel.regionalizacao reg ON reg.regid = ind.regid
            where ind.indid = $indid
            and ind.indstatus = 'A'";
            $arrDadosIndicador = $db->pegaLinha($sql);

            $complementoLinha = '';
            if (isset($request['tidid1'][$indid]) && is_array($request['tidid1'][$indid])) {
                $complementoLinha .= " and d.tidid1 in ('" . implode ("', '", $request['tidid1'][$indid]) . "') ";
            }
            if (isset($request['tidid2'][$indid]) && is_array($request['tidid2'][$indid])) {
                $complementoLinha .= " and d.tidid2 in ('" . implode ("', '", $request['tidid2'][$indid]) . "') ";
            }

            $whereLinha[] = "
                ( d.indid = {$indid}  {$complementoLinha})
            ";

            // Iniciando configuração do union de interseção
            if(!$key){
                $joinWith = $join;
                $whereLinhaWith = $whereLinha;
                $whereInicial = $where;
            } else {
                $with[] = "
                    tmp{$key} as (SELECT distinct d.dshcod, dpeanoref FROM painel.v_detalheindicadorsh d WHERE  d.indid = {$indid} {$complementoLinha} ) ";

                $whereWith .= "
                    and exists ( select 1 from tmp{$key} t where (t.dshcod, t.dpeanoref) = (d.dshcod, d.dpeanoref) )";
            }
        }

        if(is_array($with) && count($with)){
            $with = 'with ' . implode (', ', $with);
            $whereInicial .= $whereWith;

            $sqlUnion .= ' union ';
            $sqlWith .= recuperarSqlCruzamento("'Resultado dos Indicadores'", $joinWith, $whereLinhaWith, $whereInicial);
        }

        $sql = $with;
        $sql .= recuperarSqlCruzamento(' d.indnome ', $join, $whereLinha, $where);
        $sql .= $sqlUnion;
        $sql .= $sqlWith;

        $sql .= 'order by categoria';

//        ver($sql, $request);
        
        ?>
        <div class="row">
            <div class="col-md-12">
                <?php $grafico->setTitulo('Quantidade por ano')
                    ->setTipo(Grafico::K_TIPO_LINHA)
                    ->gerarGrafico($sql); ?>
            </div>
        </div>

        <div id="tab-2">
        <?php

        // Gráficos dos detalhes
        foreach($request['indicadores'] as $indid){

            $sql = "select ind.indid, indnome, ind.perid, ume.umedesc, ind.indcumulativo, ind.indcumulativovalor, ind.unmid, ind.regid,
                ind.indqtdevalor, ind.indshformula, ind.formulash
            from painel.indicador ind
                inner join painel.periodicidade per ON per.perid = ind.perid
                inner join painel.unidademeta ume ON ind.umeid = ume.umeid
                inner join painel.regionalizacao reg ON reg.regid = ind.regid
            where ind.indid = $indid
            and ind.indstatus = 'A'";
            $arrDadosIndicador = $db->pegaLinha($sql);

            $sql = "select * from painel.detalhetipoindicador where indid = $indid";
            $detalhes = $db->carregar($sql);
            $detalhes = $detalhes ? $detalhes : array();

            if(count($detalhes)){ ?>

                <div class="panel-group" role="tablist" aria-multiselectable="true">
                    <div class="panel panel-default">
                        <div class="panel-heading" role="tab" id="headinge<?php echo $indid; ?>">
                            <h4 class="panel-title">
                                <a role="button" data-toggle="collapse" data-parent="#accordion" href="#collapse<?php echo $indid; ?>" aria-expanded="true" aria-controls="collapse<?php echo $indid; ?>">
                                    <i class="fa fa-random"></i> <?php echo $arrDadosIndicador['indnome']; ?>
                                </a>
                            </h4>
                        </div>
                        <div id="collapse<?php echo $indid; ?>" class="panel-collapse collapse in" role="tabpanel" aria-labelledby="heading<?php echo $indid; ?>">
                            <div class="panel-body">

                                <div class="row">
                                    <?php foreach($detalhes as $detalhe) {
                                        $sqlDetalhe = "select tiddsc{$detalhe['tdinumero']} descricao, d.dpeanoref categoria, sum(qtde)+sum(valor) as valor
                                   from  painel.v_detalheindicadorsh d
                                        " . implode(' ', $join).  "
                                   where d.indid = {$arrDadosIndicador['indid']}
                                   and d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $periodoInicio)
                                   and d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $periodoFim)
                                   and sehstatus <> 'I'
                                   $where
                                   group by descricao, d.dpeanoref
                                   order by d.dpeanoref";
                                        ?>
                                        <div class="col-md-12">
                                            <?php $grafico->setTitulo('Quantidade por ' . $detalhe['tdidsc'])
                                                ->setTipo(Grafico::K_TIPO_COLUNA)
                                                ->gerarGrafico($sqlDetalhe); ?>
                                        </div>
                                    <?php } ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            <?php }
        } ?>
        </div>
    <?php
    }
}

function exibedashBoard2($indid){
    $_SESSION['indid'] = $indid;

    global $db;

    $sql = "select ind.indnome, exo.exodsc, sec.secdsc, aca.acadsc, ind.regid, ind.unmid, ind.indcumulativo, ind.indcumulativovalor,
				   ind.indqtdevalor, per.perdsc, per.perid
			from painel.indicador ind
                left join painel.eixo exo ON exo.exoid = ind.exoid
                left join painel.secretaria sec ON sec.secid = ind.secid
                left join painel.acao aca ON aca.acaid = ind.acaid
                left join painel.periodicidade per ON per.perid = ind.perid
			where ind.indid = $indid";
    $ind = $db->pegaLinha($sql);

    $sql = "select tdiid, tdidsc
			from painel.detalhetipoindicador
			where indid = $indid
			and tdistatus = 'A'";
    $det = $db->carregar($sql);

    ?>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr bgcolor="#f9f9f9">
            <td colspan="2" align="left">
                <span style="font-size: 16px;font-weight: bold;"><? echo $indid." - ".$ind['indnome'] ?></span><br />
                <b><? echo $ind['exodsc'] ?></b> >> <b><? echo $ind['secdsc'] ?></b> >> <b><? echo $ind['acadsc'] ?></b>
            </td>
        </tr>
        <tr>
            <td colspan="2" align="left">
                <div style="padding:5px;width:99%;border: solid 1px #000000;background-color: #FFFFFF">
                    <fieldset>
                        <legend><span style="cursor:pointer;" onclick="exibeFiltrosPainel(this);" ><img align="absmiddle" title="mais" src="../imagens/mais.gif"/> Filtros</span></legend>
                        <div id="filtros_painel" >
                            <input type="hidden" name="ultima_sehid" id="ultima_sehid" value="0" />
                            <?php if($ind['indcumulativo'] == "S"){ ?>
                                <input type="hidden" name="indcumulativo" id="indcumulativo" value="1" />
                            <?php } ?>

                            <div>
                                <div id="opc_periodo" style="margin:5px; float:left;">
                                    <fieldset style="padding:5px;">
                                        <legend>Período:</legend>

                                        <?
                                        //Carrega o período do indicador
                                        $sql = "select dpe.dpeid, dpe.dpedsc, seh.sehid
                                                from painel.seriehistorica seh
                                                    inner join painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
                                                where indid = $indid
                                                and sehstatus <> 'I'
                                                order by dpe.dpedatainicio asc";

                                        $periodos = $db->carregar($sql);

                                        if($periodos){
                                            if(count($periodos) > 12){
                                                $k1 = count($periodos) - 12;
                                            }else{
                                                $k1 = 0;
                                            }
                                            ?>
                                            <select name="periodo_inicio" id="periodo_inicio">
                                                <? foreach($periodos as $k => $periodo){ //inicio do foreach do periodo inicio?>
                                                    <option <? echo $k == $k1 ? "selected=\"selected\"" : "" ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
                                                <? } //fim do foreach do periodo inicio?>
                                            </select>
                                            <span id="ate_periodo" >até</span>
                                            <select name="periodo_fim" id="periodo_fim">
                                                <? foreach($periodos as $k => $periodo){ //inicio do foreach do periodo fim ?>
                                                    <option <? echo $k == (count($periodos) - 1)? "selected=\"selected\"" : "" ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
                                                <? } //fim do foreach do periodo fim ?>
                                            </select>
                                        <? } ?>
                                    </fieldset>
                                </div>
                                <?

                                if($ind['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $ind['unmid'] != UNIDADEMEDICAO_RAZAO):
                                    $arrPeriodicidade = getPeriodicidadeIndicador($indid);
                                    $arrPeriodicidade = !$arrPeriodicidade ? array() : $arrPeriodicidade;
                                    ?>

                                    <div id="opc_periodicidade" style="margin:5px;width:100px;float:left;">
                                        <fieldset style="padding:5px;">
                                            <legend>Periodicidade:</legend>
                                            <select id="sel_periodicidade" name="sel_periodicidade" >
                                                <?php foreach($arrPeriodicidade as $arrPer): ?>
                                                    <?php $arrPerid[] = $arrPer['perid']; ?>
                                                    <option value="<?php echo $arrPer['perid'] ?>" ><?php echo $arrPer['perdsc'] ?></option>
                                                <?php endforeach; ?>
                                                <? $arrPerid = !$arrPerid ? array() : $arrPerid ?>
                                                <?php if(!in_array(PERIODO_ANUAL,$arrPerid)): ?>
                                                    <option value="<?php echo PERIODO_ANUAL ?>" >Anual</option>
                                                <?php endif; ?>

                                            </select>
                                    </div>
                                <? endif; ?>

                                <? $sql = "	select unmdesc, i.indescala, per.perdsc
                                            from painel.indicador i
                                                left join painel.unidademedicao u on i.unmid = u.unmid
                                                left join painel.periodicidade per ON per.perid = i.perid
                                            where indid = $indid";

                                $escala = $db->pegaLinha($sql);;

                                if(strstr($escala['unmdesc'], 'Número inteiro') && $escala['indescala'] == "t"){?>

                                    <div id="opc_escala" style="margin:5px;float:left;width:150px">
                                        <fieldset style="padding:5px;">
                                            <legend>Aplicar escala em:</legend>
                                            <select id="unidade_inteiro">
                                                <option selected="selected" value="1">Unidade</option>
                                                <option value="1000">Milhares</option>
                                                <option value="1000000">Milhões</option>
                                                <option value="1000000000">Bilhões</option>
                                            </select>
                                        </fieldset>
                                    </div>
                                <? }
                                if(strstr($escala['unmdesc'], 'Moeda')){?>
                                    <div id="opc_escala_moeda" style="margin:5px;float:left;width:150px;">
                                        <fieldset style="padding:5px;">
                                            <legend>Aplicar escala em:</legend>
                                            <select id="unidade_moeda">
                                                <option selected="selected" value="1">Reais</option>
                                                <option value="1000">Milhares de Reais</option>
                                                <option value="1000000">Milhões de Reais</option>
                                                <option value="1000000000">Bilhões de Reais</option>
                                            </select>
                                        </fieldset>
                                    </div>
                                <? }
                                if(strstr($escala['unmdesc'], 'Moeda') || ($ind['indqtdevalor'] == 't')){?>
                                <div id="opc_escala_moeda" style="margin:5px;float:left;width:100px;">
                                    <fieldset style="padding:5px;">
                                        <legend>Aplicar índice:</legend>
                                        <select id="indice_moeda">
                                            <option selected="selected" value="null">Selecione...</option>
                                            <option value="ipca">IPCA Médio</option>
                                        </select>
                                    </fieldset>
                                </div>
                            </div>


                        <? }
                        if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 6 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 ||
                        $ind['regid'] == REGIONALIZACAO_POLO ||
                        $ind['regid'] == REGIONALIZACAO_IESCPC){ //Estado?>
                            <div style="clear:both">
                                <div id="opc_regiao" style="margin:5px;float:left;width:100px">
                                    <fieldset style="padding:5px;">
                                        <legend>Região:</legend>
                                        <select style="width:90px;" id="regcod" onchange="filtraEstadoDB(this.value)" >
                                            <option selected="selected" value="todos">Selecione...</option>
                                            <?
                                            $sql = "select regcod, regdescricao
                                                    from territorios.regiao
                                                    order by regdescricao";
                                            $regiao = $db->carregar($sql);

                                            foreach($regiao as $rg){?>
                                                <option value="<? echo $rg['regcod'] ?>"><? echo $rg['regdescricao'] ?></option>
                                            <? } ?>
                                        </select>
                                    </fieldset>
                                </div>

                                <div id="opc_estado" style="margin:5px;float:left;width:110px;">
                                    <fieldset style="padding:5px;">
                                        <legend>Estado:</legend>
												<span id="exibeEstado" >
													<select id="estuf" style="width:100px;" onchange="filtraMunicipio(this.value)" >
                                                        <option selected="selected" value="todos">Selecione...</option>
                                                        <?
                                                        $sql = "select estuf, estdescricao
																from territorios.estado
																order by estdescricao";
                                                        $estados = $db->carregar($sql);

                                                        foreach($estados as $uf){?>
                                                            <option value="<? echo $uf['estuf'] ?>"><? echo $uf['estdescricao'] ?></option>
                                                        <? } ?>
                                                    </select>
													</span>
                                    </fieldset>
                                </div>
                                <? } if($ind['regid'] == 7 || $ind['regid'] == 2 ||
                                    $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 ||
                                    $ind['regid'] == REGIONALIZACAO_POLO ||
                                    $ind['regid'] == REGIONALIZACAO_IESCPC){ //Município?>
                                    <div id="opc_grp_mun" style="margin:5px;float:left;width:160px;">
                                        <fieldset style="padding:5px;">
                                            <legend>Grupo de Municípios:</legend>
												<span id="exibe_grupo_municipio">
												<select id="gtmid" style="width:150px;" onchange="filtraGrupoMunicipios(this.value)" >
                                                    <option selected="selected" value="todos">Selecione...</option>
                                                    <?
                                                    $sql = "select
																gtmid, gtmdsc
															from
																territorios.grupotipomunicipio
															where
																gtmstatus = 'A'
															order by
																gtmdsc";
                                                    $grupoMun = $db->carregar($sql);
                                                    foreach($grupoMun as $gm){?>
                                                        <option value="<? echo $gm['gtmid'] ?>"><? echo $gm['gtmdsc'] ?></option>
                                                    <? } ?>
                                                </select>
												</span>
                                        </fieldset>
                                    </div>

                                    <div id="opc_tpo_mun" style="margin:5px;float:left;width:210px;">
                                        <fieldset style="padding:5px;">
                                            <legend>Tipos de Municípios:</legend>
												<span id="exibe_tipo_municipio">
												<select id="tpmid" style="width:200px;">
                                                    <option disabled="disabled" selected="selected" value="todos">Selecione...</option>
                                                </select>
												</span>
                                        </fieldset>
                                    </div>

                                    <div id="opc_mun" style="margin:5px;float:left;width:210px;">
                                        <fieldset style="padding:5px;">
                                            <legend>Município:</legend>
												<span id="exibe_municipio">
												<select id="muncod" style="width:200px;">
                                                    <option disabled="disabled" selected="selected" value="todos">Selecione...</option>
                                                </select>
												</span>
                                        </fieldset>
                                    </div>
                                    <? $sql = "	select
																regdescricao,
																regsqlcombo
															from
																painel.regionalizacao
															where
																regid = {$ind['regid']}
															and
																regsqlcombo is not null";
                                    $regDados = $db->pegaLinha($sql);
                                    if(is_array($regDados)): ?>
                                        <div id="opc_reg" style="margin:5px;float:left;width:210px;">
                                            <fieldset style="padding:5px;">
                                                <legend><?=$regDados['regdescricao'] ?>:</legend>
														<span id="exibe_reg">
														<select disabled="disabled" id="regvalue" onclick="alert('Favor Selecionar Estado e Município!')" style="width:200px;">
                                                            <option selected="selected" value="">Selecione...</option>
                                                        </select>
														</span>
                                            </fieldset>
                                        </div>
                                    <? endif; ?>
                                <? } ?>

                                <? if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){ ?>
                                <div style="clear:both">
                                    <? 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[0]['tdiid']}";
                                    $detalhe1 = $db->pegaUm($sql);
                                    ?>
                                    <div id="opc_det1" style="margin:5px;float:left;width:400px;">
                                        <fieldset style="padding:5px;">
                                            <legend><? echo $detalhe1 ?>:</legend>
                                            <?
                                            $sql = "select
														tidid as codigo,
														tiddsc as descricao
													from
														painel.detalhetipodadosindicador
													where
														tdiid = {$det[0]['tdiid']}
													and
														tidstatus = 'A'";
                                            //$db->monta_combo_multiplo('tidid1',$sql,"S","Selecione","","","",10,300,"","tidid1");
                                            //combo_popup("tidid1",$sql,"","","","","","S",true,"",5,250,"","","","","",true,"","","",$arrCarregados,"");
                                            combo_popup( "tidid1", $sql, "", '400x400', 0, array(), '', 'S', true, true );
                                            //$db->monta_combo('tidid1',$sql,'S','Selecione...','','','','200','N',"tidid1","","");
                                            ?>
                                        </fieldset>
                                    </div>
                                    <? } ?>
                                    <? if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){ ?>

                                    <? 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[1]['tdiid']}";
                                    $detalhe2 = $db->pegaUm($sql);
                                    ?>
                                    <div id="opc_det2" style="margin:5px;float:left;width:400px;">
                                        <fieldset style="padding:5px;">
                                            <legend><? echo $detalhe2 ?>:</legend>
                                            <?
                                            $sql = "select tidid as codigo, tiddsc as descricao
													from painel.detalhetipodadosindicador
													where tdiid = {$det[1]['tdiid']}
													and tidstatus = 'A'";
                                            //$db->monta_combo('tidid2',$sql,'S','Selecione...','','','','200','N',"tidid2","","");
                                            combo_popup( "tidid2", $sql, "", '400x400', 0, array(), '', 'S', true, true );
                                            ?>
                                        </fieldset>
                                    </div>
                                </div>

                            <? } ?>

                                <?php if($ind['regid'] == REGIONALIZACAO_ZONA || $ind['regid'] == REGIONALIZACAO_SUBPREFEITURA || $ind['regid'] == REGIONALIZACAO_DISTRITO || $ind['regid'] == REGIONALIZACAO_SETOR): ?>
                                <div id="opc_zona" style="margin:5px;float:left;">
                                    <fieldset style="padding:5px;">
                                        <legend>Zona:</legend>
												<span id="td_zona" >
													<select name="zonid" id="zonid" onchange="filtraSubprefeitura(this.value)" >
                                                        <option selected="selected" value="">Selecione...</option>
                                                        <?
                                                        $sql = "select
																	zonid, zondescricao
																from
																	territoriosgeo.zona
																order by
																	zondescricao";
                                                        $zona = $db->carregar($sql);
                                                        foreach($zona as $z){?>
                                                            <option <?php echo ($_POST['zonid'] == $z['zonid'] ? "selected='selected'" : "")?> value="<? echo $z['zonid'] ?>"><? echo $z['zondescricao'] ?></option>
                                                        <? } ?>
                                                    </select>
												</span>
                                    </fieldset>
                                </div>

                                <div id="opc_subprefeitura" style="margin:5px;float:left;">
                                    <fieldset style="padding:5px;">
                                        <legend>Subprefeitura:</legend>
												<span id="td_subprefeitura" >
													<select name="subid" id="subid" <?php echo $_POST['zonid'] ? "" : "disabled='disabled'" ?> onchange="filtraDistrito(this.value)" >
                                                        <option selected="selected" value="">Selecione...</option>
                                                        <?
                                                        if($_POST['zonid']){
                                                            $sql = "select
																		subid, subdescricao
																	from
																		territoriosgeo.subprefeitura
																	where
																		zonid = {$_POST['zonid']}
																	order by
																		subdescricao";
                                                            $sub = $db->carregar($sql);
                                                        }else{
                                                            $sub = array();
                                                        }
                                                        foreach($sub as $s){?>
                                                            <option <?php echo ($_POST['subid'] == $s['subid'] ? "selected='selected'" : "")?> value="<? echo $s['subid'] ?>"><? echo $s['subdescricao'] ?></option>
                                                        <? } ?>
                                                    </select>
												</span>
                                    </fieldset>
                                </div>

                                <div id="opc_distrito" style="margin:5px;float:left;">
                                    <fieldset style="padding:5px;">
                                        <legend>Distrito:</legend>
												<span id="td_distrito" >
													<select name="disid" id="disid" <?php echo $_POST['subid'] ? "" : "disabled='disabled'" ?> onchange="filtraSetor(this.value)" >
                                                        <option selected="selected" value="">Selecione...</option>
                                                        <?
                                                        if($_POST['subid']){
                                                            $sql = "select
																		disid, disdescricao
																	from
																		territoriosgeo.distrito
																	where
																		subid = {$_POST['subid']}
																	order by
																		disdescricao";
                                                            $dis = $db->carregar($sql);
                                                        }else{
                                                            $dis = array();
                                                        }
                                                        foreach($dis as $d){?>
                                                            <option <?php echo ($_POST['disid'] == $d['disid'] ? "selected='selected'" : "")?> value="<? echo $d['disid'] ?>"><? echo $d['disdescricao'] ?></option>
                                                        <? } ?>
                                                    </select>
												</span>
                                    </fieldset>
                                </div>

                                <div id="opc_setor" style="margin:5px;float:left;">
                                    <fieldset style="padding:5px;">
                                        <legend>Setor:</legend>
												<span id="td_setor" >
													<select name="setid" id="setid" <?php echo $_POST['disid'] ? "" : "disabled='disabled'" ?> >
                                                        <option selected="selected" value="">Selecione...</option>
                                                        <?
                                                        if($_POST['disid']){
                                                            $sql = "select
																		setid, setcod
																	from
																		territoriosgeo.setor
																	where
																		disid = {$_POST['disid']}
																	order by
																		setcod";
                                                            $setor = $db->carregar($sql);
                                                        }else{
                                                            $setor = array();
                                                        }
                                                        foreach($setor as $s){?>
                                                            <option <?php echo ($_POST['setid'] == $s['setid'] ? "selected='selected'" : "")?> value="<? echo $s['setid'] ?>"><? echo $s['setcod'] ?></option>
                                                        <? } ?>
                                                    </select>
												</span>
                                    </fieldset>
                                </div>

                            </div>
                        <?php endif; ?>
                        </div>
                    </fieldset>
                </div>
            </td>
        </tr>
    </table>
<? }

function recuperarSqlCruzamento($descricao, $join, $whereLinha, $where)
{
    $sql = "
            select $descricao descricao,
                   d.dpeanoref as categoria, sum(qtde)+sum(valor) as valor
            from  painel.v_detalheindicadorsh d
                " . implode(' ', $join).  "
            where sehstatus <> 'I'
            and (
                 " . implode(' or ', $whereLinha) . "
            )
            $where
            group by descricao, d.dpeanoref
            ";

    return $sql;
}

function recuperarFiltrosEscola()
{
    return array(
        'id_reg_infantil_creche', 'id_reg_infantil_preescola', 'id_reg_fund_8_anos', 'id_reg_fund_9_anos',
        'id_reg_medio_medio', 'id_reg_medio_integrado', 'id_reg_medio_normal', 'id_reg_medio_prof',
        'id_esp_infantil_creche', 'id_esp_infantil_preescola', 'id_esp_fund_8_anos', 'id_esp_fund_9_anos',
        'id_esp_medio_medio', 'id_esp_medio_integrado', 'id_esp_medio_normal', 'id_esp_medio_profissional',
        'id_esp_eja_fundamental', 'id_esp_medio_profissional',
        'id_eja_fundamental', 'id_eja_medio', 'id_eja_fundamental_projovem',
    );
}