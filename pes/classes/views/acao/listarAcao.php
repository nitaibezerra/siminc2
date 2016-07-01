
<?php global $db; ?>
<?php if ($this->values): ?>
    <table cellpadding="3" class="container_lista">
            <!--<colgroup><col width="80"><col width="50"><col><col width="80"><col width="70"><col width="70"><col width="50"></colgroup>-->
        <thead>
            <tr>
                <td>Ação</td>
                <td>Descricao</td>
                <td>Responsável</td>
                <td style="width: 20%;">Situação</td>
            </tr>
        </thead>
        <tbody>
            <?php $n = 0 ?>
            <?php foreach ($this->values as $value): ?>
                <tr class="linha_listagem">
                    <!-- Ação-->
                    <td nowrap="" style="text-align:center; width: 80px;">
                        <img onclick="javascript:carregarFormularioEditar( '<?php echo $value['acacodigo'] ?>' )" style="cursor:pointer;" align="absmiddle" title="Editar Ação" style="border: 0;" src="../imagens/editar_nome_vermelho.gif">
                        <?php if($this->save == 'S'): ?>
                        <img onclick="javascript:excluir( '<?php echo $value['acacodigo'] ?>' )" style="cursor:pointer;" align="absmiddle" title="Excluir Ação" style="border: 0;" src="../imagens/excluir.gif">
                        <?php endif ?>
                    </td>
                    <!-- Titulo-->
                    <td style="padding-left: 0px; font-weight:bold;">
                        <?php echo $value['acadescricaoacao'] ?>
                    </td>
                    <td><?php echo $value['acanomeresponsavel'] ?></td>
                    <td style="text-align: center;">
                        <select name="tidcodigo" class="CampoEstilo" <?php if($this->save == 'N') echo 'disabled' ?>>
                            <?php foreach($this->situacao as $situacao): ?>
                            <option onclick="javascript:alterarSituacao('<?php echo $situacao['codigo'] ?>', '<?php echo $value['acacodigo'] ?>');" value="<?php echo $situacao['codigo'] ?>" <?php if($value['acasituacao'] == $situacao['codigo']) echo 'selected="true"' ?>><?php echo $situacao['descricao'] ?></option>
                            <?php endforeach ?>
                        </select>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>

    <script lang="javascript">

                            /**
                             * carregarFormularioEditar
                             */
                            function carregarFormularioEditar( id ) {
                                var data = { action: 'formulario', acacodigo: id, tidcodigo: $( '#tidcodigo' ).val() };
                                var url = 'pes.php?modulo=principal/planoacao/cadastro&acao=A';

                                $.post( url, data, function( html ) {
                                    if ( $( '.container_form_save' ).css( 'display' ) == 'none' ) {
                                        $( '.container_form' ).hide().html( html ).fadeIn( 'slow' );
                                    } else {
                                        $( '.container_form' ).fadeOut().html( html ).fadeIn( 'slow' );
                                    }
                                } );
                            }

                            /**
                             * excluir
                             */
                            function excluir( id ) {
                                if ( confirm( '<?php echo MSG005 ?>' ) )
                                {
                                    $.ajax( {
                                        type: "POST",
                                        url: 'pes.php?modulo=principal/planoacao/cadastro&acao=A',
                                        data: { action: 'excluir', acacodigo: id },
                                        dataType: 'json',
                                        success: function( html ) {
                                            if ( html['status'] == true ) {
                                                alert( html['msg'] );
                                                returnSaveSucess();
                                            } else {
                                                alert( html['msg'] );
                                            }
                                        }
                                    } );
                                }
                            }

                            /**
                             * alterarSituacao
                             */
                            function alterarSituacao( acasituacao , acacodigo ) {
                                if ( confirm( '<?php echo MSG008 ?>' ) )
                                {
                                    $.ajax( {
                                        type: "POST",
                                        url: 'pes.php?modulo=principal/planoacao/cadastro&acao=A',
                                        data: { action: 'alterarSituacaoAcao', acacodigo: acacodigo, acasituacao : acasituacao },
                                        dataType: 'json',
                                        success: function( html ) {
                                            if ( html['status'] == true ) {
                                                alert( html['msg'] );
                                            } else {
                                                alert( html['msg'] );
                                            }
                                        }
                                    } );
                                }
                                
                                listar( $( '#tidcodigo' ).val() );
                            }
    </script>
<?php else: ?>
    <table style="width: 100%;">
        <tbody>
            <tr style="text-align: center;">
                <td style="font-weight: bold;">
                    Esta despesa não tem nenhuma ação cadastrada até o momento!
                </td>
            </tr>
        </tbody>
    </table>
    <!--<script lang="javascript">
        $(document).ready(function( ){
                alert('Esta despesa não tem nenhuma ação cadastrada até o momento!');
            }
        );
    </script>-->
<?php endif; ?>
