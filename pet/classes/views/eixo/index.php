<?php if (isset($_GET['id'])): ?>

    <?php require_once('form.php'); ?>

<?php else: ?>

    <div class="row well">
        <div class="col-lg-6">
            <div class="form-group">
                <label class="col-lg-4 control-label" for="nome">Selecione um Questionário:</label>

                <div class="col-lg-6">
                    <select name="queid" id="cb_queid" class="form-control">
                        <?= $this->questionario->getComboQuestionario() ?>
                    </select>
                </div>
            </div>
        </div>
    </div>
    <div id="content_form_eixo"></div>

    <script type="text/javascript">
        $(function () {
            $('#cb_queid').on('change', function () {
                var id = $(this).val();
                if (id) {
                    $.post(window.location.href, {'controller': 'eixo', 'action': 'form', 'id': id}, function (html) {
                        $('#content_form_eixo').html(html);
                    });
                } else {
                    $('#content_form_eixo').html('');
                }
            });
        })
    </script>
<?php endif; ?>