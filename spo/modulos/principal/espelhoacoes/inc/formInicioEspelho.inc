<? if($permissaoAcesso): ?>
<section class="well">
    <button class="btn btn-success buttonChange" data-url="novo"><span class="glyphicon glyphicon-new-window"></span> Novo</button>
    <button class="btn btn-primary buttonChange" data-url="listar"><span class="glyphicon glyphicon-list-alt"></span> Listar</button>
</section>
<? endif; ?>
<section class="row">
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#6cbaa8;"><strong style="color:white;">Artes Cênicas</strong></li>
            <? if($anexoAC): ?>
                <? foreach($anexoAC as $BA):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$BA['arqdata']?><br>Tamanho em Bytes: <?=$BA['arqtamanho']?><br>Responsável: <?=$BA['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$BA['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$BA['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#6cadba;"><strong style="color:white;">Artes Visuais e Artes Digitais e Eletrônicas</strong></li>
            <? if($anexoAD): ?>
                <? foreach($anexoAD as $GE):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$GE['arqdata']?><br>Tamanho em Bytes: <?=$GE['arqtamanho']?><br>Responsável: <?=$GE['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$GE['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$GE['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#ba856c;"><strong style="color:white;">Artes Integradas</strong></li>
            <? if($anexoAI): ?>
                <? foreach($anexoAI as $TE):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$TE['arqdata']?><br>Tamanho em Bytes: <?=$TE['arqtamanho']?><br>Responsável: <?=$TE['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$TE['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$TE['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#6c89ba;"><strong style="color:white;">Audiovisual</strong></li>
            <? if($anexoAV): ?>
                <? foreach($anexoAV as $SU):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$SU['arqdata']?><br>Tamanho em Bytes: <?=$SU['arqtamanho']?><br>Responsável: <?=$SU['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$SU['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$SU['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#6c89ba;"><strong style="color:white;">Humanidades</strong></li>
            <? if($anexoHM): ?>
                <? foreach($anexoHM as $SU):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$SU['arqdata']?><br>Tamanho em Bytes: <?=$SU['arqtamanho']?><br>Responsável: <?=$SU['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$SU['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$SU['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#ba856c;"><strong style="color:white;">Música</strong></li>
            <? if($anexoMU): ?>
                <? foreach($anexoMU as $TE):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$TE['arqdata']?><br>Tamanho em Bytes: <?=$TE['arqtamanho']?><br>Responsável: <?=$TE['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$TE['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$TE['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#6cbaa8;"><strong style="color:white;">Patrimônio Cultural</strong></li>
            <? if($anexoPC): ?>
                <? foreach($anexoPC as $BA):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$BA['arqdata']?><br>Tamanho em Bytes: <?=$BA['arqtamanho']?><br>Responsável: <?=$BA['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$BA['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$BA['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
    <section class="col-md-3">
        <ul class="list-group">
            <li class="list-group-item" style="background-color:#ba6c6c;"><strong style="color:white;">Outras Ações</strong></li>
            <? if($anexoOU): ?>
                <? foreach($anexoOU as $OU):?>
                <li class="list-group-item">
                    <section class="btn-group" role="group" aria-label="...">
                        <button class="btn btn-default" type="button" data-toggle="popover" data-title="Informações" data-content="Data de inclusão: <?=$OU['arqdata']?><br>Tamanho em Bytes: <?=$OU['arqtamanho']?><br>Responsável: <?=$OU['usunome']?>">
                            <span class="glyphicon glyphicon-info-sign"></span>
                        </button>
                        <button class="btn btn-default buttonDownload" type="button" data-toggle="tooltip" data-placement="bottom" title="Download"
                            data-request="&requisicao=download&arqid=<?=$OU['arqid']?>">
                            <span class="glyphicon glyphicon-download-alt"></span>
                        </button>
                    </section>
                    <?=$OU['angdsc']?>
                </li>
                <? endforeach;?>
            <? else: ?>
            <li class="list-group-item">Sem registros.</li>
            <? endif; ?>
        </ul>
    </section>
</section>