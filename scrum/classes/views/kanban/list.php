<?php
if($this->isPerfil(PERFIL_CONSULTA)){
$consulta = true;
} else {
    $consulta = false;
}
?>

<?php if ($this->list): ?>
    <div>
        <table id="scrumy-board" cellspacing="0" class="live scrum">
            <tbody id="scrumy-board-body">
                <tr id="scrumy-board-header" style="position: relative;">
                    <th id="story-header" >
                        <span>Sub-Projeto</span>
                    </th>
                    <th id="story-header">
                        <span>Estória</span>
                    </th>
                    <th id="plus-header">
                    </th>
                    <?php foreach ($this->status as $status): ?>
                        <th id="plus-header">
                            <span><?php echo $status['entstdsc'] ?></span><a href="#" class="select-all" style="display: none">Select All</a>
                        </th>
                    <?php endforeach ?>
                </tr>
                <?php foreach ($this->list as $key => $itemLista): ?>
                    <?php $n = 0 ?>
                    <?php foreach ($itemLista['filhos_subprograma'] as $keyFilhosSubprogramas => $filhosSubprograma):
                        ?>
                        <tr id="story_915989" class="story-task-row" style="position: relative; z-index: 0;  top: 0px; left: 0px;">
                            <?php if ($n < 1): ?>
                                <td rowspan="<?php echo count($itemLista['filhos_subprograma']) ?>" id="story-915989-title" class="stories">
                                    <div id="story_editor_915989" >
                                        <div class="story-handle">
                                            <span class="title">
                                                <?php echo $itemLista['nome_subprograma']; ?>
                                            </span>
                <!--                                                    <form action="/gardner_mec/save_story" class="story_editor clearfix" method="post" style="display:none"><div style="margin:0;padding:0"><input name="authenticity_token" type="hidden" value="9ea6e059f68de90cd804a0fc52b71f9b341a9d96"></div>
                                                <textarea cols="12" id="story_title" name="story[title]" rows="3">Programação Orçamentária (ELABREV)</textarea>
                                                <input id="story_id" name="story[id]" type="hidden" value="915989">
                                                <input class="dk save_button" src="/images/blank.gif?1365750341" type="image">                <input class="dk cancel_button" type="button" value="" observedactions="click"><br>
                                            </form> -->
                                        </div>
                                    </div>
                                </td>
                            <?php endif ?>
                            <td id="story-915989-title" class="stories">
                                <div id="story_editor_915989" >
                                    <div class="story-handle">
                                        <span class="title">
                                            <?php echo $filhosSubprograma['nome_estoria'] ?>
                                        </span>
            <!--                                                    <form action="/gardner_mec/save_story" class="story_editor clearfix" method="post" style="display:none"><div style="margin:0;padding:0"><input name="authenticity_token" type="hidden" value="9ea6e059f68de90cd804a0fc52b71f9b341a9d96"></div>
                                            <textarea cols="12" id="story_title" name="story[title]" rows="3">Programação Orçamentária (ELABREV)</textarea>
                                            <input id="story_id" name="story[id]" type="hidden" value="915989">
                                            <input class="dk save_button" src="/images/blank.gif?1365750341" type="image">                <input class="dk cancel_button" type="button" value="" observedactions="click"><br>
                                        </form> -->
                                    </div>
                                </div>
                            </td>
                            <td class="verify"></td>
                            <?php $classStatus = array() ?>
                            <?php foreach ($this->status as $status): ?>
                                <?php $classStatus[] = "#status_{$keyFilhosSubprogramas}_{$status['entstid']}" ?>

                                <td id="status_<?php echo $keyFilhosSubprogramas ?>_<?php echo $status['entstid'] ?>" class="status_<?php echo $status['entstid'] ?> linha_<?php echo $keyFilhosSubprogramas ?>">
                                    <?php if (isset($filhosSubprograma['filhos_estoria'][$status['entstid']])): ?>
                                        <?php foreach ($filhosSubprograma['filhos_estoria'][$status['entstid']] as $itemStatus): ?>
                                            <?php // ver($itemStatus) ?>
                                                    <!--style="background-color: <?php echo $itemStatus['subprgcolor'] ?>"-->
                                            <div class="task-kanban " id="task_<?php echo $itemStatus['entid'] ?>" style="background-color: <?php echo $itemStatus['subprgcolor'] ?>">
                                                <div class="handle"> <?php echo $itemStatus['usucpfresp_dsc']//$itemStatus['enthrsexec']   ?> </div>
                                                <div class="title" title="<?php echo $itemStatus['entdsc'] ?>" >
                                                    <pre class="title_inner"><?php echo $itemStatus['entdsc'] ?></pre>
                                                </div>

                                                <div class="iconEdit" sprintPostitId="<?php echo $itemStatus['entid'] ?>">
                                                    <i class="glyphicon glyphicon-search"></i>
                                                </div>
                                                <!--unassigned-->
                                                <div class="assignment">
                                                    <?php echo $itemStatus['enthrsexec'] ?>h
                                                </div>

                                                <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                                <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                                <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                            </div>
                                        <?php endforeach ?>
                                    <?php endif ?>
                                </td>
                            <?php endforeach ?>
                        </tr>
                    <?php if(!$consulta): ?>
                    <script lang="javascript">
                        
                        $('.iconEdit').click(function() {
                            $.post(window.location.href, {'controller': 'postit', 'action': 'detailModal', 'entid': $(this).attr('sprintPostitId')}, function(html) {
                                $('#modal').html(html).modal('show');
                            });
                        });

                        $(function() {
                            $('<?php echo implode(' , ', $classStatus) ?>').sortable({
                                connectWith: ".linha_<?php echo $keyFilhosSubprogramas ?>",
                                beforeStop: function(event, ui) {
                                    var arrStatus = $(ui.item.context).parent('td').attr('class').split(' ');
                                    var arrStatus = arrStatus[0].split('_');
                                    var idStatus = arrStatus[1];

                                    var arrPostit = ui.item.context.id.split('_')
                                    var idPostit = arrPostit[1];

                                    //                                                                console.log('Postit: ' + idPostit);
                                    //                                                                console.log('Status: ' + idStatus);

                                    dataPost = {controller : 'kanban', action: 'tramitSprint', idpostit: idPostit, idstatus: idStatus}

                                    $.post(window.location, dataPost, function(result) {
                                        console.log(result);
                                    });
                                }
                            }).disableSelection();
                        });


                        //                                                    $(function() {
                        //        $( "#sprintAnterior , #sprintAtual , #proximaSprint , #backLog" ).sortable({
                        //          connectWith: ".linha",
                        //            beforeStop: function( event, ui ) {
                        //                
                        //                var column = $(ui.item.context).parent('td');
                        //                var columnId = column.attr('id');
                        //                var postit = $(ui.item.context);
                        //                
                        //                var sprintId = column.attr('sprintId');
                        //                var sprintHoras = column.attr('sprintHoras');
                        //                var sprintHorasGastas = column.attr('sprintHorasGastas');
                        //                var sprintPostitId = postit.attr('sprintPostitId');
                        ////                console.info(sprintId);
                        //                // Pegando os IDs dos postits que estao na coluna que foi inclusa o postit
                        //                var arrColumnPostit = column.children('div .task');
                        //                var sprintArrPostitId = new Array(0);
                        //                sprintHorasGastasNovo = 0;
                        //                n = 0;
                        //                arrColumnPostit.each(function( i , element){
                        //                    if($(element).attr('sprintPostitId')){
                        //                        sprintArrPostitId[n] = $(element).attr('sprintPostitId');
                        //                        sprintHorasGastasNovo += parseInt( $(element).attr('sprintPostitHoras'));
                        //                        n++;
                        //                    }
                        //                });
                        //                
                        //                var isValid = true;
                        //                if($(event.target).attr('id') != columnId){
                        //                    if(columnId != 'backLog' && parseInt(sprintHorasGastasNovo) > parseInt(sprintHoras)){
                        //                        $( "#"+ columnId + ' , #' +$(event.target).attr('id')).sortable('cancel');
                        //                        isValid = false;
                        //                    } else {
                        //                        
                        //                        // Alterando o html th da coluna onde o postit esta.
                        //                        $('#header-' + columnId).children('.sprintHorasGastas').html(sprintHorasGastasNovo + 'hs');
                        ////                        console.info(sprintHorasGastasNovo);
                        //                        
                        //                        // Alterando o html th da coluna de  onde o postit veio.
                        //                        sprintAnteriorHorasGastasNovo = 0;
                        //                        $(event.target).children('div .task').each(function( i , element){
                        //                            sprintAnteriorHorasGastasNovo += parseInt( $(element).attr('sprintPostitHoras'));
                        //                        });
                        //                        $('#header-' + $(event.target).attr('id')).children('.sprintHorasGastas').html(sprintAnteriorHorasGastasNovo + 'hs');
                        //                    }
                        //                }
                        //                
                        //                if(isValid){
                        //                console.info(sprintArrPostitId);
                        //                    dataPost = {action: 'alterarSprint', idpostit : sprintPostitId, idsprint : sprintId, arrPostitId : sprintArrPostitId}
                        //                    $.post( window.location, dataPost, function(result) {
                        //                        console.log( result );
                        //                    });
                        //                    
                        ////                    console.info(dataPost);
                        //                } else {
                        //                    $("body").animate({ scrollTop: $('.aviso').height() }, "slow"
                        //                    , function(){
                        //                        $('.aviso').hide().empty().append('<div class="alert alert-dismissable alert-warning" ><button class="close" data-dismiss="alert" type="button">×</button><strong>Aviso!</strong><a class="alert-link" href="#"> A quantidade de horas do entregavel ultrapassa o limite do sprint, </a> retire um entregável para diminuir a quantidade de horas e tente novamente.</div></div>').fadeIn();
                        //                    });
                        //                    
                        //                    console.info('Tem muitas horas champs');
                        //                }
                        //             }
                        //        }).disableSelection();
                        //        
                        //        
                        //  });
                    </script>

                        <?php endif; ?>

                    <?php $n++ ?>
                <?php endforeach; ?>

            <?php endforeach; ?>
            </tbody>
        </table> 
        <br />
        <br />
        <br />
    </div>
<?php endif ?>
                    