<input type='hidden' id='metid' name='metid' value="<?php echo $metid;?>">

<h3 style="text-align: justify"><?php echo $mettitulo; ?></h3>

<?php foreach ($subMetas as $itemSubMeta) : ?>
    <?php $subid = $itemSubMeta['subid'];?>

    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title"><?php echo $itemSubMeta['subtitulo']; ?></h4>
        </div>
        <div class="panel-body">
            <?php $capitulos = $item->carregarItens();?>
            <?php foreach ($capitulos as $capitulo) : ?>
                <div>
                    <p style="color: #000; font-size: 12pt;"><?php echo $capitulo['itedsc']; ?></p>
                    <div class="panel-group" id="accordion">
                        <div class="avaliacao">
                            <?php $respostas = $avaliacaoC->carregarRespostasPorSubMeta($subid);?>
                            <?php $index = array_search($capitulo['iteid'], array_column($respostas, 'iteid')); ?>
                            <?php $avaliacaoIndex = $index !== false ? $respostas[$index] : array(); ?>
                            <div class="btn-group" data-toggle="buttons">
                                <?php foreach ($avaliacaoOpcoes as $opcao) : ?>
                                    <label disabled="disabled" class="btn btn-raty btn-raty-padrao <?php echo $avaliacaoIndex['avaresposta'] == $opcao['avoid'] ? Avaliacao::$niveis[$avaliacaoIndex['avaresposta']] : null; ?>">
                                        <input class="raty" type="radio" value="<?php echo $opcao['avoid'];?>" data-iteid="<?php echo $capitulo['iteid']; ?>" data-subid="<?php echo $subid; ?>" /> <?php echo $opcao['avodsc'];?>
                                    </label>
                                <?php endforeach; ?>
                            </div>
                        </div>
                    </div>
                </div>
                <hr/>
            <?php endforeach; ?>
            <div>
                <p style="color: #000; font-size: 12pt;">8. Utilize o espaço abaixo para fazer sugestões, críticas e propostas relacionadas ao indicador (máximo de 1.440 caracteres).</p>
                <div class="">
                    <textarea maxlength="1440" cols='150' disabled id="txt<?php echo $capitulo['iteid']; ?>" data-iteid="<?php echo $capitulo['iteid']; ?>" class="comment" placeholder="Informe um comentário a respeito de sua avaliação para este item" rows="4"><?php echo $avaliacaoIndex['comdsc']; ?></textarea>
                    <label id="charleft<?php echo $capitulo['iteid']; ?>">1440</label>
                </div>
            </div>
        </div>
    </div>
<?php endforeach; ?>
<div class="well-sm ">
    <div class="row" style="padding-top: 10px;">
        <div class="col-md-3 pull-left" style="padding-right: 25px;">
            <?php if ($anteriorMeta!=''):?>
            <div class="anterior-button-vizualizar">
                <input type='hidden' id='anteriorMetaVizualizar' value='<?php echo $anteriorMeta;?>'>
                <button type="button" title="Meta Anterior" class="btn btn-success">
                    <span class="fa fa-check  btn-check"></span>
                        Meta Anterior
                </button>
            </div>
            <?php endif?>
        </div>
        <div class="col-md-4 pull-right" style="padding-right: 25px;">
            <?php if ($proximaMeta!=''):?>
            <div class="proxima-button-vizualizar">
                <input type='hidden' id='proximaMetaVizualizar' value='<?php echo $proximaMeta;?>'>
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