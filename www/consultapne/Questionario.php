
<input type='hidden' id='metid' name='metid' value="<?php echo $metid;?>">

<!--<h3 style="text-align: justify" class="panel panel-default"><?php echo $mettitulo; ?></h3>-->
<div class="alert alert-warning">
    <p style="text-align: justify; font-size:18px;" class="panel-title"><?php echo $mettitulo; ?></p>
</div>

<?php foreach ($subMetas as $itemSubMeta) : ?>
    <?php $subid = $itemSubMeta['subid'];?>

    <div class="panel panel-danger">
        <div class="panel-heading panelMeta">
            <?php $indicador = 18 == $metid ? 'Indicador 18' : $itemSubMeta['subtitulo']; ?>
            <h4 class="panel-title fontMeta"><?php echo $indicador; ?></h4>
        </div>
        <div class="panel-body">
            <?php $capitulos = $item->carregarItens();?>
            <?php foreach ($capitulos as $capitulo) : ?>

                <?php
                    if(18 == $metid && 8 != $capitulo['iteid']){
                        continue;
                    }
                    $itedsc = 18 == $metid ? 'Utilize o espaço abaixo para fazer sugestões, críticas e propostas relacionadas ao desenvolvimento de indicador(es) para acompanhamento desta Meta (máximo de 1.440 caracteres).' : $capitulo['itedsc'];
                ?>

                <div>
                    <p style="color: #000; font-size: 12pt;"><?php echo $itedsc; ?></p>
                    <?php $respostas = $avaliacaoC->carregarRespostasPorSubMeta($subid);?>
                    <?php $index = array_search($capitulo['iteid'], array_column($respostas, 'iteid')); ?>
                    <?php $avaliacaoIndex = $index !== false ? $respostas[$index] : array(); ?>
                    <?php if( $capitulo['itetipo'] != 'T'){ ?>
                        <div class="panel-group" id="accordion">
                            <div class="avaliacao">
                                <div class="btn-group" data-toggle="buttons">
                                    <?php foreach ($avaliacaoOpcoes as $opcao) : ?>
                                        <label class="btn btn-raty btn-raty-padrao <?php echo $avaliacaoIndex['avaresposta'] == $opcao['avoid'] ? Avaliacao::$niveis[$avaliacaoIndex['avaresposta']] : null; ?>">
                                            <input class="raty" type="radio" value="<?php echo $opcao['avoid'];?>" data-iteid="<?php echo $capitulo['iteid']; ?>" data-subid="<?php echo $subid; ?>" /> <?php echo $opcao['avodsc'];?>
                                        </label>
                                    <?php endforeach; ?>
                                </div>
                            </div>
                        </div>
                    <?php } else { ?>
                        <div>
                            <textarea cols='150' id="txt<?php echo $subid; ?>" data-iteid="<?php echo $capitulo['iteid']; ?>" data-subid="<?php echo $subid; ?>" class="comment" placeholder="Informe um comentário a respeito de sua avaliação para este item" rows="4"><?php echo $avaliacaoIndex['comdsc']; ?></textarea>
                            <label id="charleft<?php echo $subid; ?>">1440</label> Caracteres Restantes
                        </div>
                    <?php } ?>
                </div>
                <hr/>
            <?php endforeach; ?>
        </div>
    </div>
<?php endforeach; ?>
<div class="well-sm" style="padding: 9px 0">

    <div class="row" style="padding-top: 10px;">
        <div class="col-lg-4 col-sm-4 col-xs-4" style="padding-left: 0">
            <?php if ($anteriorMeta!=''):?>
            <div class="anterior-button pull-left">
                <input type='hidden' id='anteriorMeta' value='<?php echo $anteriorMeta;?>'>
                <button type="button" title="Meta Anterior" class="btn btn-success center-block">
                    <span class="fa fa-check  btn-check"></span> 
                        Meta Anterior
                </button>
            </div>
            <?php endif?>
        </div>
        <div class="col-lg-4 col-sm-4 col-xs-4">
                <button title="Finalizar" class="btn btn-danger center-block">
                    <span class="fa"></span> Finalizar Preenchimento
                </button>
            </h2>
        </div>        
        <div class="col-lg-4 col-sm-4 col-xs-4" style="padding-right: 0">
            <?php if ($proximaMeta!=''):?>
            <div class="proxima-button pull-right">
                <input type='hidden' id='proximaMeta' value='<?php echo $proximaMeta;?>'>
                <button type="button" title="Próxima Meta" class="btn btn-success">
                    <span class="fa fa-spinner fa-spin btn-loading" style="display: none;"></span>
                    <span class="fa fa-check  btn-check"></span> 
                        Próxima Meta
                </button>
            </div>
            <?php endif?>
        </div>
    </div>                       
</div>