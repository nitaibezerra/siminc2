<?php

// Programa
$program = $this->program;

// Sprints
$sprintPrevious = $this->sprintPrevious;
$sprintCurrent = $this->sprintCurrent;
$sprintNext = $this->sprintNext;

// Postits
$postitSprintPrevious = $this->postitSprintPrevious;
$postitSprintCurrent = $this->postitSprintCurrent;
$postitSprintNext = $this->postitSprintNext;
$postitBackLog = $this->postitBackLog;

$sprintPreviousHours = 0;
if (is_array($sprintPrevious)) {
    foreach ($postitSprintPrevious as $postit) {
        $sprintPreviousHours += $postit['enthrsexec'];
    }
}

$sprintCurrentHours = 0;
if (is_array($sprintCurrent)) {
    foreach ($postitSprintCurrent as $postit) {
        $sprintCurrentHours += $postit['enthrsexec'];
    }
}

$sprintNextHours = 0;
if (is_array($postitSprintNext)) {
    foreach ($postitSprintNext as $postit) {
        $sprintNextHours += $postit['enthrsexec'];
    }
}

if($this->isPerfil(PERFIL_CONSULTA)){
    $consulta = true;
} else {
    $consulta = false;
}
?>
<br />
<div class="row">
    <div>
        <table id="scrumy-board" cellspacing="0" class="live scrum">
            <tbody id="scrumy-board-body">
                <tr id="scrumy-board-header"  style="position: relative;">
<!--                                    <th id="story-header" >
                        <span>Subprograma</span>
                    </th>
                    <th id="story-header">
                        <span>Estória</span>
                    </th>
                    <th id="plus-header">
                    </th>-->
                    <th id="header-sprintAnterior" class="header">
                        <span>Ciclo anterior</span> <br />
                        <?php echo $sprintPrevious['sptinicio'] ?> - <?php echo $sprintPrevious['sptfim'] ?> <br />
                        (<span class="sprintHorasGastas"><?php echo $sprintPreviousHours ?>hs</span>/ <?php echo $program['prghrsprint'] ?>hs )
                        <a href="#" class="select-all" style="display: none">Select All</a>
                    </th>
                    <th id="header-sprintAtual" class="header">
                        <span>Ciclo atual</span> <br />
                        <?php echo $sprintCurrent['sptinicio'] ?> - <?php echo $sprintCurrent['sptfim'] ?> <br />
                        (<span class="sprintHorasGastas"><?php echo $sprintCurrentHours ?>hs</span>/ <?php echo $program['prghrsprint'] ?>hs )
                        <a href="#" class="select-all" style="display: none">Select All</a>
                    </th>
                    <th id="header-proximaSprint" class="header">
                        <span>Próximo ciclo</span> <br />
                        <?php echo $sprintNext['sptinicio'] ?> - <?php echo $sprintNext['sptfim'] ?> <br />
                        (<span class="sprintHorasGastas"><?php echo $sprintNextHours ?>hs</span>/ <?php echo $program['prghrsprint'] ?>hs )
                        <a href="#" class="select-all" style="display: none">Select All</a>
                    </th>
                    <th id="header-backLog" class="header">
                        <span>Backlog</span>
                        <a href="#" class="select-all" style="display: none">Select All</a>
                    </th>
                </tr>
                <tr class="story-task-row">
                    <td id="sprintAnterior" class="linha status_3" sprintId="<?php echo $sprintPrevious['sptid'] ?>" sprintHorasGastas="<?php echo $sprintPreviousHours ?>" sprintHoras="<?php echo $program['prghrsprint'] ?>">
                        <?php if ($postitSprintPrevious): foreach ($postitSprintPrevious as $postit):
                            if($postit['entstid'] == ENTREGAVEL_STATUS_PRONTO)
                                $postit['subprgcolor'] = '#CCC';
                            ?>
                                <?php
                                if ($postit['usucpfresp_dsc']) {
                                    $nomeResponsavel = explode(' ' , $postit['usucpfresp_dsc']);
                                    $nomeResponsavel = $nomeResponsavel[0];
                                } else {
                                    $nomeResponsavel = '';
                                }

                                ?>

                                <div sprintPostitId="<?php echo $postit['entid'] ?>" sprintPostitHoras="<?php echo $postit['enthrsexec'] ?>" class="task" style="background-color: <?php echo $postit['subprgcolor'] ?>">
                                    <div class="handle"> <?php echo $postit['subprgdsc']//$itemStatus['enthrsexec']   ?> </div>
                                    <div class="title"  >
                                        <pre class="title_inner" style="<?php if($postit['entstid'] == ENTREGAVEL_STATUS_PRONTO) echo 'background-color: #f0f0f0; color:#9F9F9F' ?>">

                                        <b><?php echo $postit['esttitulo'] ?></b>

                                        <?php echo $postit['entdsc'] ?>
                                        </pre>
                                    </div>
                                    <div class="iconEdit" sprintPostitId="<?php echo $postit['entid'] ?>">
                                        <i class="glyphicon glyphicon-pencil"></i>
                                    </div>
                            <?php if($nomeResponsavel): ?>
<!--                                    <div class="iconUserOn">-->
<!--                                        <i class="glyphicon glyphicon-user"></i>-->
<!--                                    </div>-->
                                    <div class="iconUser">
                                        <i style="font-size: 20px" class="glyphicon glyphicon-user"></i>
                                    </div>
                                    <div class="user"><?php echo $nomeResponsavel ?></div>
                            <?php endif ?>
                                    <!--unassigned-->
                                    <div class="assignment">
                                        <?php echo $postit['enthrsexec'] ?>hs
                                    </div>

                                    <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                    <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                    <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                </div>
                                <?php // ver($entregavelBackLog);  ?>
                                <?php
                            endforeach;
                        endif;
                        ?>
                    </td>
                    <td id="sprintAtual" class="linha status_5" sprintId="<?php echo $sprintCurrent['sptid'] ?>" sprintHorasGastas="<?php echo $sprintCurrentHours ?>" sprintHoras = "<?php echo $program['prghrsprint'] ?>">
                        <?php if ($postitSprintCurrent): foreach ($postitSprintCurrent as $postit):
                            if($postit['entstid'] == ENTREGAVEL_STATUS_PRONTO)
                                $postit['subprgcolor'] = '#CCC';
                            ?>
                                <?php
                                if ($postit['usucpfresp_dsc']) {
                                    $nomeResponsavel = explode(' ' , $postit['usucpfresp_dsc']);
                                    $nomeResponsavel = $nomeResponsavel[0];
                                } else {
                                    $nomeResponsavel = '';
                                }
                                ?>

                                <div sprintPostitId="<?php echo $postit['entid'] ?>" sprintPostitHoras="<?php echo $postit['enthrsexec'] ?>" class="task" style="background-color: <?php echo $postit['subprgcolor'] ?>">
                                    <div class="handle"> <?php echo $postit['subprgdsc']//$itemStatus['enthrsexec']    ?> </div>
                                    <div class="title" >
                                        <pre class="title_inner" style="<?php if($postit['entstid'] == ENTREGAVEL_STATUS_PRONTO) echo 'background-color: #f0f0f0; color: #9F9F9F;' ?>">

<b><?php echo $postit['esttitulo'] ?></b>

    <?php echo $postit['entdsc'] ?>
                                        </pre>
                                    </div>
                                    <div class="iconEdit" sprintPostitId="<?php echo $postit['entid'] ?>">
                                        <i class="glyphicon glyphicon-pencil"></i>
                                    </div>
                            <?php if($nomeResponsavel): ?>
<!--                                <div class="iconUserOn">-->
<!--                                    <i class="glyphicon glyphicon-user"></i>-->
<!--                                </div>-->
                                    <div class="iconUser">
                                        <i style="font-size: 20px" class="glyphicon glyphicon-user"></i>
                                    </div>
                                    <div class="user"><?php echo $nomeResponsavel ?></div>
                            <?php endif ?>
                                    <!--unassigned-->
                                    <div class="assignment">
                                        <?php echo $postit['enthrsexec'] ?>hs
                                    </div>

                                    <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                    <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                    <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                </div>
                                <?php // ver($entregavelBackLog);  ?>
                                <?php
                            endforeach;
                        endif;
                        ?>
                    </td>
                    <td id="proximaSprint" class="linha status_2"  sprintId = "<?php echo $sprintNext['sptid'] ?>" sprintHorasGastas="<?php echo $sprintNextHours ?>" sprintHoras = "<?php echo $program['prghrsprint'] ?>">
                        <?php if ($postitSprintNext): foreach ($postitSprintNext as $postit):
                            if($postit['entstid'] == ENTREGAVEL_STATUS_PRONTO)
                                $postit['subprgcolor'] = '#CCC';
                            ?>
                                <?php
                                if ($postit['usucpfresp_dsc']) {
                                    $nomeResponsavel = explode(' ' , $postit['usucpfresp_dsc']);
                                    $nomeResponsavel = $nomeResponsavel[0];
                                } else {
                                    $nomeResponsavel = '';
                                }
                                ?>

                                <div sprintPostitId="<?php echo $postit['entid'] ?>" sprintPostitHoras="<?php echo $postit['enthrsexec'] ?>" class="task" style="background-color: <?php echo $postit['subprgcolor'] ?>">
                                    <div class="handle"> <?php echo $postit['subprgdsc']//$itemStatus['enthrsexec']    ?> </div>
                                    <div class="title" >
                                        <pre class="title_inner" style="<?php if($postit['entstid'] == ENTREGAVEL_STATUS_PRONTO) echo 'background-color: #f0f0f0; color: #9F9F9F' ?>">

<b><?php echo $postit['esttitulo'] ?></b>

    <?php echo $postit['entdsc'] ?>
                                        </pre>
                                    </div>
                                    <!--unassigned-->
                                    <div class="iconEdit" sprintPostitId="<?php echo $postit['entid'] ?>">
                                        <i class="glyphicon glyphicon-pencil"></i>
                                    </div>
                            <?php if($nomeResponsavel): ?>
<!--                                <div class="iconUserOn">-->
<!--                                    <i class="glyphicon glyphicon-user"></i>-->
<!--                                </div>-->
                                    <div class="iconUser">
                                        <i style="font-size: 20px" class="glyphicon glyphicon-user"></i>
                                    </div>
                                    <div class="user"><?php echo $nomeResponsavel ?></div>
                            <?php endif ?>
                                    <div class="assignment">
                                        <?php echo $postit['enthrsexec'] ?>hs
                                    </div>
                                    <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                    <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                    <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                </div>
                                <?php // ver($entregavelBackLog);  ?>
                                <?php
                            endforeach;
                        endif;
                        ?>
                    </td>
                    <td id="backLog" class="linha status_1" sprintId="" sprintHorasGastas="" sprintHorasLimite="">
                        <?php if ($postitBackLog): foreach ($postitBackLog as $postit): ?>
                                <?php
                                if ($postit['usucpfresp_dsc']) {
                                    $nomeResponsavel = explode(' ' , $postit['usucpfresp_dsc']);
                                    $nomeResponsavel = $nomeResponsavel[0];
                                } else {
                                    $nomeResponsavel = '';
                                }
                                ?>
                                <!--background-color: rgba(255, 0, 0, 0.5)-->
                                <div sprintPostitId="<?php echo $postit['entid'] ?>" sprintPostitHoras="<?php echo $postit['enthrsexec'] ?>" class="task" style="background-color: <?php echo $postit['subprgcolor'] ?>">
                                    <div class="handle"> <?php echo $postit['subprgdsc']//$itemStatus['enthrsexec']   ?> </div>
                                    <div class="title" >
                                        <pre class="title_inner">

<b><?php echo $postit['esttitulo'] ?></b>

    <?php echo $postit['entdsc'] ?>
                                        </pre>
                                    </div>
                                    <div class="iconEdit" sprintPostitId="<?php echo $postit['entid'] ?>">
                                        <i class="glyphicon glyphicon-pencil"></i>
                                    </div>
                                    <?php if($nomeResponsavel): ?>
<!--                                        <div class="iconUserOn">-->
<!--                                            <i class="glyphicon glyphicon-user"></i>-->
<!--                                        </div>-->
                                        <div class="iconUser">
                                            <i style="font-size: 20px" class="glyphicon glyphicon-user"></i>
                                        </div>
                                        <div class="user"><?php echo $nomeResponsavel ?></div>
                                    <?php endif ?>
                                    <!--unassigned-->
                                    <div class="assignment">
                                        <?php echo $postit['enthrsexec'] ?>hs
                                    </div>
                                    <!--<a href="#" class="edit-task" observedactions="click">Change</a>-->
                                    <!--<a href="/gardner_mec/destroy_task?id=2298313" class="delete-task" observedactions="click">Delete</a>-->


                                    <img class="overflow_indicator" src="/images/overflow_indicator.gif">
                                </div>
                                <?php // ver($entregavelBackLog);    ?>
                                <?php
                            endforeach;
                        endif;
                        ?>
                    </td>
                </tr>
            </tbody>
        </table>
        <br />
        <br />
        <br />
    </div>
    <?php if(!$consulta): ?>
    <script lang="javascript">

        $('.iconEdit').click(function() {
            $.post(window.location.href, {'controller': 'postit', 'action': 'formularioModal', 'entid': $(this).attr('sprintPostitId')}, function(html) {
                $('#modal').html(html).modal('show');
            });
        });

        $(function() {
            $("#sprintAnterior , #sprintAtual , #proximaSprint , #backLog").sortable({
                connectWith: ".linha",
                beforeStop: function(event, ui) {

                    var column = $(ui.item.context).parent('td');
                    var columnId = column.attr('id');
                    var postit = $(ui.item.context);

                    var sprintId = column.attr('sprintId');
                    var sprintHoras = column.attr('sprintHoras');
                    var sprintHorasGastas = column.attr('sprintHorasGastas');
                    var sprintPostitId = postit.attr('sprintPostitId');
                    //                console.info(sprintId);
                    // Pegando os IDs dos postits que estao na coluna que foi inclusa o postit
                    var arrColumnPostit = column.children('div .task');
                    var sprintArrPostitId = new Array(0);
                    sprintHorasGastasNovo = 0;
                    n = 0;
                    arrColumnPostit.each(function(i, element) {
                        if ($(element).attr('sprintPostitId')) {
                            sprintArrPostitId[n] = $(element).attr('sprintPostitId');
                            sprintHorasGastasNovo += parseInt($(element).attr('sprintPostitHoras'));
                            n++;
                        }
                    });

                    var isValid = true;
                    if ($(event.target).attr('id') != columnId) {
                        if (columnId != 'backLog' && parseInt(sprintHorasGastasNovo) > parseInt(sprintHoras)) {
                            $("#" + columnId + ' , #' + $(event.target).attr('id')).sortable('cancel');
                            isValid = false;
                        } else {

                            // Alterando o html th da coluna onde o postit esta.
                            $('#header-' + columnId).children('.sprintHorasGastas').html(sprintHorasGastasNovo + 'hs');
                            //                        console.info(sprintHorasGastasNovo);

                            // Alterando o html th da coluna de  onde o postit veio.
                            sprintAnteriorHorasGastasNovo = 0;
                            $(event.target).children('div .task').each(function(i, element) {
                                sprintAnteriorHorasGastasNovo += parseInt($(element).attr('sprintPostitHoras'));
                            });
                            $('#header-' + $(event.target).attr('id')).children('.sprintHorasGastas').html(sprintAnteriorHorasGastasNovo + 'hs');
                        }
                    }

                    if (isValid) {
                        console.info(sprintArrPostitId);
                        dataPost = {controller: 'postit', action: 'changeSprint', idpostit: sprintPostitId, idsprint: sprintId, arrPostitId: sprintArrPostitId}
                        $.post(window.location, dataPost, function(result) {
                            console.log(result);
                        });

                        //                    console.info(dataPost);
                    } else {
                        $("body").animate({scrollTop: $('.aviso').height()}, "slow"
                                , function() {
                            $('.aviso').hide().empty().append('<div class="alert alert-dismissable alert-warning" ><button class="close" data-dismiss="alert" type="button">×</button><strong>Aviso!</strong><a class="alert-link" href="#"> A quantidade de horas do entregavel ultrapassa o limite do sprint, </a> retire um entregável para diminuir a quantidade de horas e tente novamente.</div></div>').fadeIn();
                        });

                        console.info('Tem muitas horas champs');
                    }
                }
            }).disableSelection();


        });

    </script>
<?php endif ?>